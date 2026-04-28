#!/usr/bin/env perl
use strict;
use warnings;
use utf8;
use open qw(:std :utf8);
use feature 'say';
use File::Basename;

# Usage check
if (@ARGV < 2) {
  die "Usage: perl map_media_to_links.pl <data.tsv> <drivelinks.tsv> [output.tsv]\n";
}

my ($data_tsv, $drivelinks_tsv, $output_file) = @ARGV;
$output_file //= "mapped_output.tsv";

die "Data TSV file does not exist: $data_tsv\n" unless -f $data_tsv;
die "Drivelinks TSV file does not exist: $drivelinks_tsv\n" unless -f $drivelinks_tsv;

say "=" x 60;
say "STEP 1: Loading drive links mapping";
say "=" x 60;

# Load drivelinks.tsv - first column is filename, second is link
open my $links_fh, "<:encoding(UTF-8)", $drivelinks_tsv or die "Cannot open drivelinks TSV: $!\n";

my %drive_links;
my $links_loaded = 0;

while (my $line = <$links_fh>) {
  chomp $line;
  $line =~ s/\r//g;
  
  my @fields = split /\t/, $line;
  next unless @fields >= 2;
  
  my $filename = $fields[0];
  my $link = $fields[1];
  
  # Clean filename (remove quotes, get basename)
  $filename =~ s/^["']//g;
  $filename =~ s/\s//g;
  $filename = basename($filename);
  
  $drive_links{$filename} = $link;
  $links_loaded++;
}

close $links_fh;

say "Loaded $links_loaded drive links";

say "\n" . "=" x 60;
say "STEP 2: Processing data TSV and mapping media files";
say "=" x 60;

open my $data_fh, "<:encoding(UTF-8)", $data_tsv or die "Cannot open data TSV: $!\n";
open my $out_fh, ">:encoding(UTF-8)", $output_file or die "Cannot create output file: $!\n";

my $line_num = 0;
my $mapped_count = 0;
my $unmapped_count = 0;

while (my $line = <$data_fh>) {
  chomp $line;
  $line =~ s/\r//g;
  $line_num++;
  
  my $modified_line = $line;
  my %replacements;
  
  # Find all media file patterns in the line
  while ($line =~ /(\S+\.(jpg|jpeg|png|gif|mp4|mov|avi|mkv|webm|mp3|wav))/gi) {
    my $match = $1;
    my $cleaned = $match;
    $cleaned =~ s/["']//g;
    my $filename = basename($cleaned);
    
    if (exists $drive_links{$filename}) {
      # Store replacement: original match -> drive link
      $replacements{$cleaned} = $drive_links{$filename};
      $mapped_count++;
    } else {
      $unmapped_count++;
      if ($unmapped_count <= 10) {
        warn "  No drive link found for: $filename\n";
      }
    }
  }
  
  # Apply replacements
  for my $old (keys %replacements) {
    my $new = $replacements{$old};
    # Escape special regex characters in the original match
    $modified_line =~ s/$old/$new/g;
  }
  
  say $out_fh $modified_line;
}

close $data_fh;
close $out_fh;

say "\n" . "=" x 60;
say "SUMMARY";
say "=" x 60;
say "Lines processed: $line_num";
say "Media files mapped to links: $mapped_count";
say "Media files without links: $unmapped_count";
say "Output written to: $output_file";
say "=" x 60;
