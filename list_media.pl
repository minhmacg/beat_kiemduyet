#!/usr/bin/env perl
use strict;
use warnings;
use utf8;
use open qw(:std :utf8);
use feature 'say';
use File::Find;
use File::Basename;

# Usage check
if (@ARGV < 1) {
  die "Usage: perl list_media_folder.pl <source_media_dir>\n";
}

my $source_dir = shift;

die "Source directory does not exist: $source_dir\n" unless -d $source_dir;

say "=" x 60;
say "Scanning directory: $source_dir";
say "=" x 60;

my %ext_count;
my %all_files;
my $total_files = 0;
my @sample_files;

find(sub {
  return unless -f;
  $total_files++;
  
  my $basename = basename($_);
  $all_files{$basename} = $File::Find::name;
  
  # Collect first 20 files as samples
  push @sample_files, "$basename => $File::Find::name" if @sample_files < 20;
  
  # Count by extension
  if ($basename =~ /\.([^.]+)$/i) {
    my $ext = lc($1);
    $ext_count{$ext}++;
  } else {
    $ext_count{'(no extension)'}++;
  }
}, $source_dir);

say "Total files found: $total_files";
say "\nFiles by extension:";
for my $ext (sort { $ext_count{$b} <=> $ext_count{$a} } keys %ext_count) {
  say sprintf("  %-20s: %d", ".$ext", $ext_count{$ext});
}

say "\nFirst 20 files found:";
say "  $_" for @sample_files;

# Count specific media extensions
my @media_exts = qw(jpg jpeg png gif mp4 mov avi mkv webm mp3 wav);
my $media_count = 0;

for my $ext (@media_exts) {
  $media_count += ($ext_count{$ext} // 0);
}

say "\n" . "=" x 60;
say "Media files (jpg|jpeg|png|gif|mp4|mov|avi|mkv|webm|mp3|wav): $media_count";
say "Other files: " . ($total_files - $media_count);
say "=" x 60;
