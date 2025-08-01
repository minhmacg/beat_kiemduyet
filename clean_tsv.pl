use strict;
use warnings;
use feature 'say';
use open qw(:std :encoding(UTF-8));
use utf8;
my $onlylike = "--onlylike" ~~ @ARGV;
my @lines;
{
	open my $f , "<:encoding(UTF-8)", $ARGV[0] or die "Can't open $ARGV[1]";
	while (my $line = <$f>) 
	{
		my @fields = split /\t/, $line;
		push @lines, \@fields;
	};
};
my @kdv = ("Tuấn Dũng", "Nguyễn Hưng", "Minh Vũ", "Dương Nguyễn");
my @result = grep {
	$onlylike ?
		$_->[8] ne ""
		: $_->[8] ne ""|| $_->[2] ~~ @kdv || $_->[6] ne ""
} @lines;
for my $line (@result)
{
	print STDOUT (join "\t", @$line); 
};

