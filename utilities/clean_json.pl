use strict;
use warnings;
use JSON;
use utf8;

binmode STDOUT, ':encoding(UTF-8)';
binmode STDERR, ':encoding(UTF-8)';

my $file = do
{
	open my $fh, '<', $ARGV[0] or die "Can't open $ARGV[0]";
	local $/;
	<$fh>;
};
my $json;
eval {
	$json = decode_json($file);
};
if ($@) {
    my $err = $@;
    warn "❌ JSON parse error:\n$err";

    # Try to extract the character offset from the error
    if ($err =~ /at character offset (\d+)/) {
        my $offset = $1;

        # Count line number and show the line
        my $before = substr($file, 0, $offset);
        my @lines = split /\n/, $before;
        my $line_num = scalar @lines;
        my $bad_line = (split /\n/, $file)[$line_num];

        warn "→ Error likely at line $line_num:\n";
        warn "$bad_line\n" if defined $bad_line;
    }

    return 0;
}
print STDERR "✅ Valid JSON\n";


my @result;
my %seen;
my $dupcount = 0;

foreach my $entry (@$json) {
    my $key = encode_json($entry);
    if (!$seen{$key}++) {
        push @result, $entry;
    } else {
        $dupcount++;
    }
}

my $size = scalar @$json;
my $kdv = "";

use IO::Handle;
STDERR->autoflush(1);
for (my $i = 0; $i < @result; $i++) {
    print STDERR "\rProcessing " . ($i + 1) . "/$size";
    my $user = $result[$i]->{user} // "";
    $result[$i]->{user} = "" if !defined $user || $user eq "User N/A";

    $result[$i]->{kdv} = $kdv;
    $result[$i]->{user} =~ s/ đã gửi \d* ảnh//g;
    $result[$i]->{user} =~ s/Icon for this message.*//g;
}
print STDERR "\n";

my $coder = JSON->new->pretty(1);
print $coder->encode($json);
print STDERR "Dup count : $dupcount\n";

