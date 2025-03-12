set links (jq -r '.[].media | select(. | length > 0) | .[].src' $argv[1])
set total (count $links)
set foldername (echo $argv[1] | sed 's/\.json//')

mkdir -p ../medias/$foldername/photos
mkdir -p ../medias/$foldername/videos

set index 1

for link in $links
	set filename (echo $link | grep -Eo '[^/]+\.(jpg|png|mp4|jpeg)')
	set dst

	if string match -q '*.mp4' $filename
		set dst (echo "../medias/$foldername/videos/$filename")
	else
		set dst (echo "../medias/$foldername/photos/$filename")
	end

	echo "[$index/$total] Downloading: $filename"
	curl -o $dst $link

	set index (math $index + 1)
end

