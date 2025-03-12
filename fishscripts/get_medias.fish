argparse 'i=' 'o=' -- $argv

if not set -q _flag_i
	echo "Usage: script -i <inputfile> [-o <outputfile>]"
	exit
end

if not set -q _flag_o
	echo "Usage: script -i <inputfile> [-o <outputfile>]"
	exit
end
rg -oN 'https?:\/\/\S*\/([0-9n_]+\.(jpg|png|mp4))[\"^\S]*' $_flag_i > $_flag_o
bat $_flag_o
