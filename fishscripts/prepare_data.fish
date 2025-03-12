# Download media

cd ~/Documents/beat/
pwd
cd raw_json/
pwd
for file in *json
	fish ../fishscripts/download_media.fish $file
	fish ../fishscripts/clean_1.fish $file > ../json_src/$file
	echo $file
end
