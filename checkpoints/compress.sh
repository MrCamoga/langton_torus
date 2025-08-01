for f in *; do
	if [ -d "$f" ]; then
		tar cjf "$f.tar.bz2" "$f";
	fi;
done
