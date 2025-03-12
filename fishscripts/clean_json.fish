set file (cat $argv[1] | jq -r 'map(del(.node))')

# Get the length of the filtered file
set length (echo $file | jq 'length')

# Initialize an empty list to store the final results
set final_results

# Loop through each object in the filtered file
for i in (seq 0 (math "$length - 1"))
    # Get the current object
    set current_object (echo $file | jq ".[$i]")

    # Get the user field of the current object
    set current_user (echo $current_object | jq -r '.user')

	if test -z $current_user -o "Mở ảnh" = $current_user
        # If react is empty, check for the first non-empty user field
        for j in (seq (math $i + 1) (math $length - 1))
            set next_object (echo $file | jq ".[$j]")
            set next_user (echo $next_object | jq -r '.user')
            
            if test -n $next_user -a "Mở ảnh" != $next_user
                # If we find a non-empty user, copy it to the current object
                set current_object (echo $current_object | jq ".user = \"$next_user\"")
                break
            end
        end

    end
	set final_results $final_results $current_object
end

echo $final_results | jq -s .
