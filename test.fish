function echotest
	for i in (seq 1 10)
		echo hihi + $i
	end
end

function t
	echo aaa
	cat
end

echotest | t
