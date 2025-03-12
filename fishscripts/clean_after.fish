argparse 'i=' 'o=' -- $argv

if not set -q _flag_i
	echo "Usage: script -i <inputfile> [-o <outputfile>]"
end

for line in (cat $_flag_i)
	set pagename (echo $line | cut -f2)
end

#clean links
sed -E "s/https?:\/\/\S*\/([0-9n_]+\.(jpg|png|mp4))[^\"[:space:]]*/\1/g" $_flag_i
