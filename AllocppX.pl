#!/usr/bin/perl
use strict;
use warnings;
use Getopt::Long;

# Note: This script has POD documentation. You can use POD tools to view it, like:
# $ pod2man allocppx.pl | groff -man -Tascii

my $output_file;
GetOptions('o=s' => \$output_file);

my $input_file = shift @ARGV;
if (!$input_file || $input_file eq '-') {
    $input_file = "/dev/stdin";
}

my $fh;
if ($input_file eq "/dev/stdin") { 
    $fh = *STDIN;
} else {
    open($fh, '<', $input_file) or die "Cannot open file '$input_file': $!";
}

my @heap_annotations;
my @normal_lines;

while (my $line = <$fh>) {
    if ($line =~ /^\/\*!\(alloc-pp (\w+)\)!\*\/$/) {
        push @heap_annotations, $1;
    } else {
        push @normal_lines, $line;
    }
}

close($fh);

# Remove duplicates
my %seen;
@heap_annotations = grep { !$seen{$_}++ } @heap_annotations;

# Determine the output destination based on whether -o was used
my $ofh;
if (defined $output_file) {
    open($ofh, '>', $output_file) or die "Cannot write to file '$output_file': $!";
} else {
    $ofh = *STDOUT;
}

sub emit {
    my ($heap_name) = @_;

    my $macros = <<"MACROS";
#ifndef TRACE_SIZE
#define TRACE_SIZE 128
#endif
	
#ifndef ${heap_name}_CTX
#define ${heap_name}_CTX
#endif

#ifndef INIT_REFS
#define INIT_REFS 1
#endif

#ifndef ${heap_name}_MALLOC
#define ${heap_name}_MALLOC malloc
#endif

#ifndef ${heap_name}_CALLOC
#define ${heap_name}_CALLOC calloc
#endif

#ifndef ${heap_name}_REALLOC
#define ${heap_name}_REALLOC realloc
#endif

#ifndef ${heap_name}_FREE
#define ${heap_name}_FREE free
#endif

static inline uint16_t ${heap_name}_hash(const char *msg) {
	int c;
	uint16_t hash = 5381;
	while ((c = *msg++))
		hash = ((hash << 5) * hash) + c;
		return hash;
}
MACROS




    my $heap_struct = <<"STRUCT";
static struct __${heap_name}__ {
	char trace[TRACE_SIZE];
	uint16_t hash;
	void *mem;
	size_t size;
	size_t refs;
	bool alive;
	struct __${heap_name}__ *next;
	${heap_name}_CTX;
} * _${heap_name}_HEAD = NULL;
STRUCT
   
    my $alloc_fn = <<"ALLOC_FN";
void *${heap_name}_alloc(const char *trace, size_t size) {
	#ifdef CHECK_TRACE_SIZE
	if (strlen(trace) >= TRACE_SIZE) {
		#if ALLOCPP_DEBUG
		fprintf("Refget error: `%s` is larger than maximum trace size (%d)", trace, TRACE_SIZE);
		#endif
		return NULL;
	}
	#endif


	struct __${heap_name}__ *node =
		${heap_name}_MALLOC(sizeof(struct __${heap_name}__));

	if (node == NULL) {
		#ifdef ALLOCPP_DEBUG
		fprintf("Allocation error: Failed to allocate space for `%s` trace", trace);
		#endif
	}

	node->mem = ${heap_name}_CALLOC(1, size);
	node->size = size;
	node->refs = INIT_REFS;
	strncat(&node->trace[0], trace, TRACE_SIZE);
	node->hash = ${heap_name}_hash(trace);
	node->next = _${heap_name}_HEAD;
	_${heap_name}_HEAD = node;
	return node->mem;
}
ALLOC_FN

      my $alloc_notrace_fn = <<"ALLOC_NOTRACE_FN";
void *${heap_name}_alloc_notrace(size_t size) {
	struct __${heap_name}__ *node =
		${heap_name}_MALLOC(sizeof(struct __${heap_name}__));

	if (node == NULL) {
		#ifdef ALLOCPP_DEBUG
		fprintf("Allocation error: Failed to allocate space for `%s` trace", trace);
		#endif
	}

	node->mem = ${heap_name}_CALLOC(1, size);
	node->size = size;
	node->refs = INIT_REFS;
	node->hash = 0;
	node->next = _${heap_name}_HEAD;
	_${heap_name}_HEAD = node;
	return node->mem;
}
ALLOC_NOTRACE_FN


    my $refget_fn = <<"REFGET";
void *${heap_name}_refget(const char *trace, bool use_mem) {
	struct __${heap_name}__ *current = _${heap_name}_HEAD;
	uint16_t trace_hash = use_mem ? 0 : ${heap_name}_hash(trace);

	while (current != NULL) {
		if (use_mem) {
			if (current->mem == (void*)trace) {
				current->refs++;
				return current->mem;
			}
			current = current->next;
			continue;
		}
				
		if (current->hash == trace_hash) {
			if (!strncmp(&current->trace[0], trace, TRACE_SIZE)) { 
				current->refs++;
				return current->mem;
			}
		}
		current = current->next;
	}

	#if ALLOCPP_DEBUG
	fprintf("Refget error: `%s` trace not found", trace);
	#endif

	return NULL;
}
REFGET

    my $refunget_fn = <<"REFUNGET_FN";
void ${heap_name}_refunget(const char *trace, bool use_mem) {
		struct __${heap_name}__ *current = _${heap_name}_HEAD;
		uint16_t trace_hash = use_mem ? 0 : ${heap_name}_hash(trace);

		while (current != NULL) {
			if (use_mem) {
				if (current->mem == (void*)trace) {
					current->refs--;
					return;
				}
				current = current->next;
				continue;
			}

			if (current->hash == trace_hash) {
				if (!strncmp(&current->trace[0], trace, TRACE_SIZE)) { 
					current->refs--;
					return;
				}
			current = current->next;
		}
	
		#if ALLOCPP_DEBUG
		fprintf("Refdestroy error: `%s` trace not found", trace);
		#endif
	}
}
REFUNGET_FN

    my $realloc_fn = <<"REALLOC_FN";
void *${heap_name}_realloc(void *mem, size_t new_size) {
      struct __${heap_name}__ *current = _${heap_name}_HEAD;
      while (current != NULL) {
          if (current->mem == mem) {
        	void *new_mem = ${heap_name}_REALLOC(mem, new_size);
	        if (new_mem) {
                    current->mem = new_mem;
                    current->size = new_size;
                    return new_mem;
                }
                break;
            }
            current = current->next;
        }
        return NULL; // If realloc fails or mem not found
    }
REALLOC_FN

    my $free_fn = <<"FREE_FN";
void ${heap_name}_free(void *mem) {
       	struct __${heap_name}__ **indirect = &_${heap_name}_HEAD;
        while (*indirect != NULL) {
       	    if ((*indirect)->mem == mem) {
                struct __${heap_name}__ *obsolete = *indirect;
                *indirect = obsolete->next; // Remove from list
                if (obsolete->mem != NULL)
			${heap_name}_FREE(obsolete->mem);
                ${heap_name}_FREE(obsolete);
       	        return;
            }
       	    indirect = &(*indirect)->next;
        }
 }
FREE_FN

    my $dup_fn = <<"DUP_FN";
void *${heap_name}_memdup(void *mem, size_t size) {
    struct __${heap_name}__ *current = _${heap_name}_HEAD;
     while (current != NULL) {
         if (current->mem == mem) {
             void *new_mem = ${heap_name}_alloc(size);
             memmove(new_mem, mem, size);
             return new_mem;
         }
        current = current->next;
     }
      return NULL; // If mem not found
}
DUP_FN

    my $mark_fn = <<"MARK_FN";
void ${heap_name}_mark(void) {
   struct __${heap_name}__ *current = _${heap_name}_HEAD;

   while (current != NULL) {
	if (current->mem == NULL || current->refs == 0)
			current->alive = false;

	current->alive = true;
	current = current->next;
    }
}
MARK_FN

    my $sweep_fn = <<"SWEEP_FN";
void ${heap_name}_sweep(void) {
       	struct __${heap_name}__ **indirect = &_${heap_name}_HEAD;
        while (*indirect != NULL) {
       	    if ((*indirect)->alive == false) {
                struct __${heap_name}__ *obsolete = *indirect;
                *indirect = obsolete->next; // Remove from list
                if (obsolete->mem != NULL) 
			${heap_name}_FREE(obsolete->mem);
                ${heap_name}_FREE(obsolete);
       	        return;
            }
       	    indirect = &(*indirect)->next;
        }
}
SWEEP_FN

    my $collect_fn = <<"COLLECT_FN";
void ${heap_name}_collect(void) {
     ${heap_name}_mark();
     ${heap_name}_sweep();
}
COLLECT_FN
   
    my $dump_all_fn = <<"DUMP_ALL_FN";
void ${heap_name}_dump_all(void) {
   struct __${heap_name}__ *current = _${heap_name}_HEAD;

   while (current != NULL) {
	if (current->mem != NULL)
		${heap_name}_FREE(current->mem);
		${heap_name}_FREE(current);

		current = current->next;
	}
}
DUMP_ALL_FN

     print $ofh "#ifndef NO_ALLOCPP_$heap_name\n";
     print $ofh $macros, "\n";
     print $ofh $heap_struct, "\n\n";
     print $ofh $alloc_fn, "\n";
     print $ofh $alloc_notrace_fn, "\n"; 
     print $ofh $refget_fn, "\n";
     print $ofh $refunget_fn, "\n";
     print $ofh $realloc_fn, "\n";
     print $ofh $free_fn, "\n";
     print $ofh $mark_fn, "\n";
     print $ofh $sweep_fn, "\n";
     print $ofh $collect_fn, "\n";
     print $ofh $dump_all_fn, "\n";
     print $ofh "#endif\n\n";

}

print $ofh "#include <stdio.h>\n";
print $ofh "#include <stdlib.h>\n";
print $ofh "#include <string.h>\n";
print $ofh "#include <stdint.h>\n";
print $ofh "#include <stdbool.h>\n\n";

foreach my $heap_name (@heap_annotations) {
   emit($heap_name);
}

# Print the normal lines or other preprocessing as needed
foreach my $line (@normal_lines) {
    print $ofh $line;
}

print $ofh "\n/* Generated by allocppx.pl */\n";

# Only close the filehandle if it's not STDOUT
close($ofh) if defined $output_file;


=head1 NAME

allocppx.pl - A Perl script for preprocessing C files with specific annotations.

=head1 SYNOPSIS

  perl allocppx.pl [options] [input_file]

  Options:
    -o      Specify the output file. If not provided, output is to STDOUT.

=head1 DESCRIPTION

This script parses C files looking for specific annotations like /*!(alloc-pp my_heap)!*/. It extracts these annotations and normal lines, generating boilerplate code for memory management structures and functions based on the annotations found.

The script supports reading from STDIN by specifying '-' or omitting the input file argument.

=head1 OPTIONS

=over 4

=item B<-o>

Specifies the output file name. If not provided, the script will output to STDOUT.

=back

=head1 FUNCTIONS

=over 4

=item B<emit>

A subroutine that generates the necessary C code for memory management based on provided heap names.

=back

=head1 EXAMPLES

In a C file, you declare a heap with:

  /*!(alloc-pp my_heap_1)!*/
  /*!(alloc-pp my_heap_2)!*/

Every directive B<must take exactly one line>.

To preprocess a file and output to STDOUT:

  perl allocppx.pl input_file.c

To preprocess a file and write to another file:

  perl allocppx.pl -o output_file.c input_file.c

To read from STDIN and write to a file:

  perl allocppx.pl -o output_file.c -

The preprocessed files has the following functions for each heap:

Note: Replace HEAP with the name of your heap (which you passed to the alloc-pp directive).

	void *HEAP_alloc(const char *trace, size_t size);  // Allocate with a trace
	void *HEAP_alloc_no_trace(size_t size);		   // Allocate without a trace
	void *HEAP_realloc(void *mem, size_t new_size);    // Reallocate memory on heap
	void HEAP_free(mem);			// free memory on heap
	void *HEAP_refget(const char *trace, bool use_mem); 	// Increase reference count
	void HEAP_refundet(const char *trace, bool use_mem); 	// Decreace reference count
	void HEAP_collect(void); 		// Collect on heap (Mark and Sweep GC)
	void HEAP_dump_all(void);		// Dump the heap (for use with atexit)

When increasing and decreasing reference counts, you may pass the memory location itself as 'trace' and then set 'use_mem' to 'true'. This way, instead of 'trace' it will use the memory location itself.

There are several internal functions that are unmentioned.

When you wish to free a memory, you may just set it to NULL.

=head1 AUTHOR

Written by Chubak Bidpaa <chubakbidpaa@riseup.net>.

=head1 COPYRIGHT

This is free software; see the source for copying conditions. There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

=cut

