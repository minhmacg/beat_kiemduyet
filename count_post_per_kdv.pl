binmode STDOUT, ':encoding(UTF-8)';
use Text::CSV;
my $csv = Text::CSV->new({
    binary => 1, 
    sep_char => "\t",
    quote_char => undef,
    escape_char => undef
});
my %seen;
open my $f, '<:encoding(UTF-8)', $ARGV[0] or die '';
while (my $row = $csv->getline($f)) {
  my @fields = @$row;
  if (defined $fields[8] && $fields[8] eq "1")
  {
	  $seen{$fields[7]}++;
 };
}

print "$_ ", $seen{$_}, "\n" for sort keys %seen;
