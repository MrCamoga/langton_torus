for f in *; do
	if [[ -d "$f" ]]; then
		mv "$f".sav.* "$f"/;
	fi;
done
