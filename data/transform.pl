#!/usr/bin/perl

$zmin = 0;
while(<>)
{
	push @input, $_;
	if(/^v ([-\d\.]+) ([-\d\.]+) ([-\d\.]+)/)
	{
		if($3 < $zmin) { $zmin = $3; }
	}
}
foreach (@input)
{
	if(/^v ([-\d\.]+) ([-\d\.]+) ([-\d\.]+)/)
	{
		$_ = "v $1 $2 " . ($3 - $zmin) . "\n";
	}
	print;
}
	

