#!/usr/bin/env perl
use strict;
use warnings;
use utf8;
use open qw(:std :utf8);
use feature 'say';
use File::Basename;

# Usage check
if (@ARGV < 1) {
  die "Usage: perl debug_tsv_parsing.pl <tsv_file>\n";
}

my $tsv_file = shift;
die "TSV file does not exist: $tsv_file\n" unless -f $tsv_file;

say "=" x 60;
say "Parsing TSV file: $tsv_file";
say "=" x 60;

open my $fh, "<:encoding(UTF-8)", $tsv_file or die "Cannot open TSV: $!\n";

my %files_found;
my %ext_count;
my $line_num = 0;
my @sample_matches;

while (my $line = <$fh>) {
  $line_num++;
  chomp $line;
  
  # Show first few lines
  if ($line_num <= 5) {
    my $preview = substr($line, 0, 100);
    $preview .= "..." if length($line) > 100;
    say "Line $line_num: $preview\n";
  }
  
  # Find all media file patterns
  while ($line =~ /(\S+\.(jpg|jpeg|png|gif|mp4|mov|avi|mkv|webm|mp3|wav))/gi) {
    my $match = $1;
    my $ext = lc($2);
    my $filename = basename($match);
    
    $files_found{$filename}++;
    $ext_count{$ext}++;
    
    # Keep some samples
    if (@sample_matches < 20) {
      push @sample_matches, "Line $line_num: $match => $filename";
    }
  }
}

close $fh;

say "\n" . "=" x 60;
say "RESULTS";
say "=" x 60;
say "Total lines in TSV: $line_num";
say "Unique media files found: " . scalar(keys %files_found);

say "\nFiles by extension:";
for my $ext (sort { $ext_count{$b} <=> $ext_count{$a} } keys %ext_count) {
  say sprintf("  %-10s: %d", ".$ext", $ext_count{$ext});
}

if (@sample_matches) {
  say "\nFirst 20 matches found:";
  say "  $_" for @sample_matches;
}

# Check for duplicates
my @duplicates = grep { $files_found{$_} > 1 } keys %files_found;
if (@duplicates) {
  say "\nDuplicate files (appear multiple times):";
  for my $i (0 .. 9) {
    last unless $i < @duplicates;
    say "  $duplicates[$i]: $files_found{$duplicates[$i]} times";
  }
}

say "\n" . "=" x 60;
say "SUMMARY";
say "=" x 60;
say "Total unique media files: " . scalar(keys %files_found);
say "=" x 60;
