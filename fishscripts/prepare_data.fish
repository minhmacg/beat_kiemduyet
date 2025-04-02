# Download media

cd raw_json/
pwd
for file in *json
	set output ../json_src/$file
	../utilities/clean_json -i $file > $output
	fish ../fishscripts/download_media_from_json.fish $output
	echo $file
end
