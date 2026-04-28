use strict;
use warnings;
use Text::CSV_XS;
use feature 'say';
use open qw(:std :encoding(UTF-8));
use utf8;
my @kdv = ("Tuấn Dũng", "Nguyễn Hưng", "Minh Vũ", "Dương Nguyễn");

my $csv = Text::CSV_XS->new({
    binary        => 1,
    sep_char      => "\t",
    allow_loose_quotes => 1,
    allow_loose_escapes => 1,
	eol => "\n",
	always_quote => 1,
    auto_diag     => 1,
});
my @lines;
{
	open my $f , "<:encoding(UTF-8)", $ARGV[0] or die "Can't open $ARGV[0]";
	while (my $line = $csv->getline($f)) 
	{
		next unless defined $line->[8] && $line->[8] !~ /^\s*$/ && not grep {$_ eq $line->[2]} @kdv;
		$csv->print(*STDOUT, $line);
	};
};

