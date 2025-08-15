# Download media

cd raw_json/
pwd
for file in *json
	set output ../json_src/$file
	perl ../utilities/clean_json.pl $file > $output
	fish ../fishscripts/download_media_from_json.fish $output
	echo $file
end
