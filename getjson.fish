argparse 'd=' -- $argv

if not set -q _flag_d
	echo "Usage: getjson -d <facebook_downloaded_folder>"
end

set -l names 1gkdfbchuyencuahanoi \
  antoanthxsmacheckbaiquangcao \
  chotchanbeatnetwork \
  chotchanshowbeat1g \
  chottiktokhtbz \
  finaltiktokbeatvn2025 \
  kdbeatnow \
  kdfbbeatvnfinal \
  kdfbcaothu \
  kdfbsaigonnghenn \
  kdkienkhongngu \
  kdkienkhongngutrending \
  kdtiktokcchn \
  kdtiktokshowbeat \
  kdttbeat_thegame \
  kiemduyetbeatviralworld \
  kiemduyetinsidethebox \
  kiemduyettthellovietnam \
  kiemduyettthongheart \
  sanphamxkiemduyet

rm json_src/*
for dir in (ls -1 $_flag_d/your_facebook_activity/messages/inbox/)
	set -l pagename (echo $dir | perl -pe "s/_\d+\$//")
	if contains $pagename $names
		echo $pagename
		jq . $_flag_d/your_facebook_activity/messages/inbox/$dir/message_1.json \
			| iconv -f UTF-8 -t ISO-8859-1 > json_src/$pagename.json
	end
end

for json in (ls json_src)
	set -l pagename (echo $json | perl -pe "s/\.json\$//")
	perl beatdata.pl json_src/$json > output/$pagename.tsv
end
