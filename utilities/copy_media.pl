#!/usr/bin/env perl
use strict;
use warnings;
use utf8;
use open qw(:std :utf8);
use feature 'say';
use File::Find;
use File::Copy;
use File::Spec;
use File::Basename;

# Usage check
if (@ARGV < 3) {
  die "Usage: perl copy_media_files.pl <tsv_file> <source_media_dir> <destination_dir>\n";
}

my ($tsv_file, $source_dir, $dest_dir) = @ARGV;

# Validate directories
die "Source directory does not exist: $source_dir\n" unless -d $source_dir;
die "TSV file does not exist: $tsv_file\n" unless -f $tsv_file;

# Create destination directory if it doesn't exist
unless (-d $dest_dir) {
  mkdir $dest_dir or die "Cannot create destination directory: $!\n";
}

# Build index of all media files in source directory
say "Building index of media files in $source_dir...";
my %media_index;
find(sub {
  return unless -f;
  return unless /\.(jpg|jpeg|png|gif|mp4|mov|avi|mkv|webm|mp3|wav)$/i;
  my $basename = basename($_);
  push @{$media_index{$basename}}, $File::Find::name;
}, $source_dir);

say "Found " . scalar(keys %media_index) . " unique media filenames";

# Parse TSV and collect media files with simple pattern matching
say "Parsing TSV file...";
open my $fh, "<:encoding(UTF-8)", $tsv_file or die "Cannot open TSV: $!\n";

my %files_to_copy;
my $line_num = 0;

while (my $line = <$fh>) {
  $line_num++;
  chomp $line;
  
  # Find all media file patterns in the line
  while ($line =~ /(\S+\.(jpg|jpeg|png|gif|mp4|mov|avi|mkv|webm|mp3|wav))/gi) {
    my $match = $1;
	$match =~ s/^["']+//;
    my $filename = basename($match);
    $files_to_copy{$filename} = 1;
  }
}

close $fh;

say "Found " . scalar(keys %files_to_copy) . " media files referenced in TSV";

# Copy files
my $copied = 0;
my $not_found = 0;

say "\nCopying files...";
for my $filename (sort keys %files_to_copy) {
  if (exists $media_index{$filename}) {
    my $source_path = $media_index{$filename}[0]; # Use first match
    my $dest_path = File::Spec->catfile($dest_dir, $filename);
    
    if (-e $dest_path) {
      say "  Skip (exists): $filename";
      next;
    }
    
    if (copy($source_path, $dest_path)) {
      say "  Copied: $filename";
      $copied++;
    } else {
      warn "  Failed to copy $filename: $!\n";
    }
    
    # Warn if multiple matches exist
    if (@{$media_index{$filename}} > 1) {
      warn "  Warning: Multiple copies found for $filename, used first match\n";
    }
  } else {
    $not_found++;
  }
}

say "\n" . "=" x 60;
say "Summary:";
say "  Files copied: $copied";
say "  Files not found: $not_found";
say "  Destination: $dest_dir";
say "=" x 60;
