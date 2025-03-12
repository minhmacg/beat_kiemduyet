argparse 'h/help' 'i=' 'o=' -- $argv
if set -q _flag_h
	echo "Usage: script -i <input_file> -o <output folder>"
	exit 0
end

if not set -q _flag_i
	echo "Usage: script -i <input_file> -o <output folder>"
	exit 0
end

if not set -q _flag_o
	echo "Usage: script -i <input_file> -o <output folder>"
	exit 0
end

set links (cat $_flag_i)
set total (count $links)
mkdir -p $_flag_o/photos
mkdir -p $_flag_o/videos

set index 1

for link in $links
	if test -z $link
		continue
	end
	set filename (echo $link | grep -Eo '[^/]+\.(jpg|png|mp4|jpeg)')
	set dst

	if string match -q '*.mp4' $filename
		set dst (echo "$_flag_o/videos/$filename")
	else
		set dst (echo "$_flag_o/photos/$filename")
	end

	echo "[$index/$total] Downloading: $filename"
	curl -o $dst $link

	set index (math $index + 1)
end

