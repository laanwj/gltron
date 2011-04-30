#!/usr/bin/perl -w

use Gimp;
# use X11::Protocol;

exit Gimp::main();

sub net {

  set_trace(TRACE_ALL);
#  $x = X11::Protocol->new();

  $author = "Andreas Umbach";
  $copyright = "Andreas Umbach (c)";
  $date = localtime;

  $font = "-ttf-xenotron-medium-*-*-regular-*-*-*-*-*-*-*-*";
  $name = "xenotron";
  $lower = 32;
  $upper = 126;

  $tw = 256;
  $w = 32;

  $border = 1;
  $h = $w - 2 * $border;

## 
##   $fid = $x->new_rsrc;
##   $x -> req('OpenFont', $fid, $font);
##   print "font: $fid\n";
##   %fontinfo = $x->req('QueryFont', $fid);
## 
##   $chars = $fontinfo{"char_infos"};
##   print "char width:\n";
##   foreach $char (@$chars) {
##     print $$char[3], "\n";
##   }
##   exit(1);
## 
  $imgcount = 0;

  &createImage;

  $x = 0; $y = 0;
  for($i = 0; $i <= $upper - $lower; $i++) {
    $x += $w;
    if($x + $w > $tw) {
      $x = 0;
      $y += $w;
      if($y + $w > $tw) {
	# create second texture
	print("font does not fit in - creating second image\n");
	push @drws, $drw;
	&createImage;

	$imgcount++;
	$y = 0;
      }
    }

    print "($x, $y): '" . chr($i + $lower) . "'\n";
    next if($i + $lower == 32);
    # get text extents to center char...
    ($width,$height,$ascent,$descent) =
      gimp_text_get_extents_fontname(chr($i + $lower), $h, 1, $font);
    $px = $x + ($w - $width) / 2;
    $py = $y + ($w - $height) / 2;

    $text_layer = gimp_text_fontname($drw, $px, $py, chr($i + $lower),
				     $border, 1, $h, 1, $font);
    gimp_floating_sel_anchor($text_layer);
  }

  push @drws, $drw;
  for($i = 0; $i <= $imgcount; $i++) {
    $drw = $drws[$i];
    $fname = "/tmp/" . $name . ".$i" . ".sgi";
    file_sgi_save($drw, $fname, $fname, 0);
  }
}

sub createImage {
  $img = gimp_image_new($tw, $tw, RGB);
  $drw = gimp_layer_new($img, $img->width, $img->height ,RGB, "BG", 100,
			NORMAL_MODE);
  $drw->add_alpha();
  $drw->add_layer(-1); # -1 means at top of stack

  gimp_palette_set_background("black");
  gimp_edit_fill($drw);
  $img->selection_all();
  $drw->edit_clear();

  gimp_palette_set_foreground([255, 255, 255]); # "white"
}












