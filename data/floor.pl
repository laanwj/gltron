print "mtllib floor.mtl\n# object pPlane2\ng pPlane2\n";

$min = -1002;
$max = 1002;
$width = 11;

$offset = ($max - $min) / ($width - 1);

for($y = $min; $y <= $max + 0.5 * $offset; $y+=$offset)
{
	for($x = $min; $x <= $max + 0.5 * $offset; $x+=$offset)
	{
		print "v $x $y 0\n";
	}
}
print "vn 0 0 1\n";
for($y = $min; $y <= $max + 0.5 * $offset; $y+=$offset)
{
	for($x = $min; $x <= $max + 0.5 * $offset; $x+=$offset)
	{
		print "vt ", ($x - $min) / ($max - $min), " ",
			($y - $min) / ($max - $min), "\n";
	}
}

for($y = 0; $y < $width - 1; $y++)
{
	for($x = 0; $x < $width - 1; $x++)
	{
		$base = $x + $y * $width + 1;
		print "f ";
		print $base, "/", $base, "/1 ";
		print $base + 1, "/", $base + 1, "/1 ";
		print $base + $width + 1, "/", $base + $width + 1, "/1\n";
		print "f ";
		print $base, "/", $base, "/1 ";
		print $base + $width + 1, "/", $base + $width + 1, "/1 ";
		print $base + $width, "/", $base + $width, "/1\n";
	}
}

