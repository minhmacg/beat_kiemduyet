use strict;
use warnings;
use feature 'say';
use open qw(:std :encoding(UTF-8));
use utf8;
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
my @liked_and_has_content = grep {
	$_->[8] ne "" ||
	$_->[2] ~~ @kdv ||
	$_->[6] ne ""	
} @lines;
for my $line (@liked_and_has_content)
{
	print STDERR $line->[2], " ", $line->[8], "\n";
	print STDOUT (join "\t", @$line); 
};
