use strict;
use warnings;
use utf8;
binmode(STDOUT, ":encoding(UTF-8)");
use Encode qw(encode decode);
use JSON;
use POSIX 'strftime';

if (scalar @ARGV != 1) {die "Usage: beatdata <jsonfile>"};

my %pagename;
{
    open my $f, '<:encoding(UTF-8)', 'pagename' or die "can't open pagename";
    while (<$f>)
    {
        chomp;
        my ($p, $page) = split ',', $_;
        $pagename{$p} = $page;
    };
    close $f;
}
my %kdperpage = (
	'1gkdfbchuyencuahanoi' => "Vũ",
	antoanthxsmacheckbaiquangcao => "Dũng",
	chotchanbeatnetwork => "Vũ",
	chotchanshowbeat1g => "Dũng",
	chottiktokhtbz => "Hưng",
	finaltiktokbeatvn2025 => "Dũng",
	kdbeatnow => "Hưng",
	kdfbbeatvnfinal => "Dũng",
	kdfbcaothu => "Hưng",
	kdfbsaigonnghenn => "Dương",
	kdkienkhongngu => "Dương",
	kdkienkhongngutrending => "Hưng",
	kdtiktokcchn => "Hưng",
	kdtiktokshowbeat => "Vũ",
	kdttbeat_thegame => "Hưng",
	kiemduyetbeatviralworld => "Hưng",
	kiemduyetinsidethebox => "Vũ",
	kiemduyettthellovietnam => "Dương",
	kiemduyettthongheart => "Hưng",
	sanphamxkiemduyet => "Dũng"
);

my $json = do {
    open my $f, '<', $ARGV[0] or die "Can't open json $ARGV[0]";
    local $/;
    my $json_text = <$f>;
    close $f;
    
	# Fix facebook byte encoding
    $json_text =~ s/\\u00([0-9a-f]{2})/chr(hex($1))/ge;
    #$json_text =~ s/\\x5cu([0-9a-f]{4})/pack("C", hex($1) & 0xff)/ige;
    
    decode_json($json_text);
};

my %btv_status;

foreach my $p (@{$json->{participants}}) {$btv_status{$p->{name}} = {posting => 0, start_timestamp => 0, anchor => undef};};

foreach my $m (@{$json->{messages}})
{
	# Do some clean up
	if ($m->{content} && $m->{share})
	{
		if ($m->{content} eq $m->{share}->{link}) {$m->{content} = ""};
	};

	if ($m->{content} && $m->{content} ne "")
	{
		$m->{content} =~ s/\n|\t|\r/ /g;
		#$m->{content} =~ s/“|”/"/g;
		#$m->{content} =~ s/^"+|"+$//g;
		$m->{content} = '"' . $m->{content} . '"';
	};
	my $current_btv_status = $btv_status{$m->{sender_name}};
	if ($current_btv_status->{posting} &&
		$current_btv_status->{start_timestamp} - $m->{timestamp_ms} > 30000)
	{
		$current_btv_status->{posting} = 0;
	};

	next if (!$m->{reactions});
	for my $react (@{$m->{reactions}})
	{
		if ($react->{reaction} eq "👍")
		{
			$m->{kdv} = $react->{actor};
		};
	};
	# If not already posting, start
	if (!$current_btv_status->{posting})
	{
		$current_btv_status->{posting} = 1;
		$current_btv_status->{start_timestamp} = $m->{timestamp_ms};
		$current_btv_status->{anchor} = $m;
		$m->{anchor} = 1;
	}
	elsif (!$m->{consumed})
	{
		my $anchor = $current_btv_status->{anchor};
		my $consumed = 0;
		if (!$anchor->{content} && $m->{content}) { $anchor->{content} = $m->{content}; $consumed = 1;}
		if ($m->{photos})
		{
			if ($anchor->{photos}) {push @{$anchor->{photos}}, @{$m->{photos}}; }
			else {$anchor->{photos} = $m->{photos}; };
			$consumed = 1;
		};
		if ($m->{videos})
		{
			if ($anchor->{videos}) {push @{$anchor->{videos}}, @{$m->{videos}}; }
			else {$anchor->{videos} = $m->{videos}; };
			$consumed = 1;
		};
		if (!$anchor->{share} && $m->{share}) { $anchor->{share} = $m->{share}; $consumed = 1;};

		if ($consumed) {$m->{consumed} = 1};
	};
};

my @filtered_messages = reverse (grep { !$_->{consumed} } @{$json->{messages}});

sub string_from_media
{
	my ($media) = @_;
	my $rs = "";
	if ($media)
	{
		$rs .= "\"";
		foreach my $p (@{$media})
		{
			$p->{uri} =~ /([^\/]+\..+$)/;
			$rs .= "$1\n";
		};
		$rs .= "\"";
	}
	$rs;
};


foreach my $m (@filtered_messages)
{
	my $time = strftime("%d/%m/%y", localtime($m->{timestamp_ms}/1000));
	my $page = $ARGV[0] =~ s/(.*\/)?([^\/]+)\.json/$2/r;
	$page = $pagename{$page};
	my $kq = $m->{reactions} ? "1" : "";

	my $photos = string_from_media($m->{photos});
	my $videos = string_from_media($m->{videos});

	for ($time, $page, $m->{sender_name}, $m->{content}, $photos, $videos, $m->{share}->{link}, $m->{kdv}, $kq)
	{
		my $field = defined $_ ? $_ : "";
		print $field . "\t";
	};
	print "\n";
};
#my $output = JSON->new->pretty->encode(\@filtered_messages);
#print $output;

