# Download media

cd raw_json/
pwd
for file in *json
	fish ../fishscripts/download_media_from_json.fish $file
	fish ../fishscripts/clean_json.fish $file > ../json_src/$file
	echo $file
end
