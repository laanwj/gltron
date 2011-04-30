#!/usr/bin/perl -w

use Gimp;
# use X11::Protocol;

exit Gimp::main();

sub net {

  set_trace(TRACE_ALL);

  $author = "Andreas Umbach";
  $copyright = "Andreas Umbach (c)";
  $date = localtime;

  $font = "-ttf-xenotron-medium-*-*-regular-*-*-*-*-*-*-*-*";
  $name = "xenotron";

  $tw = 512;
  $fname = "/tmp/crash.tex";

  &createImage;

  $circles = 2;
  $r = $tw / (2 * $circles + 1);
  gimp_palette_set_foreground([255, 0, 0]);
  print "radius: $r\n";
  for($i = $circles; $i > 0; $i--) {
    drawCircle($img, $drw, $tw / 2, $tw, $i * $r, 2);
  }
  gimp_palette_set_foreground("white");
  drawCircleFill($img, $drw, $tw / 2, $tw, $r / 4);

  $pi = 3.14125;

  $sparcs = 12;
  $sparc_length = $r * 1.625;
  $sparc_d = $r * 0.375;
  $sparc_w = $pi / 20;
  $sparc_s = $r / 4;
  $a = $pi / ($sparcs - 1);

  gimp_palette_set_foreground("white");

  for($i = 0; $i <= $sparcs; $i++) {
    $length = $sparc_length + (rand(2) - 1) * $sparc_d;
    $vertex[0] = $tw / 2 - $sparc_s * cos($a * $i - $sparc_w);
    $vertex[1] = $tw     - $sparc_s * sin($a * $i - $sparc_w);
    $vertex[2] = $tw / 2 - $sparc_s * cos($a * $i + $sparc_w);
    $vertex[3] = $tw     - $sparc_s * sin($a * $i + $sparc_w);
    $vertex[4] = $tw / 2 - $length * cos($a * $i);
    $vertex[5] = $tw     - $length * sin($a * $i);

    $vertex[6] = $vertex[0];
    $vertex[7] = $vertex[1];

    drawTriangle($img, $drw, \@vertex);
  }
  file_sgi_save($drw, $fname, $fname, 0);
}

sub drawTriangle {
  my ($img, $drw, $vertices) = @_;
  gimp_selection_none($img);
  $img->free_select(8, $vertices, 0, 0, 0, 0);
  gimp_bucket_fill($drw, 0, 0, 100, 0, 0, 0, 0);
}
sub drawCircle {
  my ($img, $drw, $x, $y, $r, $w) = @_;
  gimp_selection_none($img);
  gimp_ellipse_select($img, $x - $r, $y - $r, $r * 2, $r * 2,
			0, 1, 0, 0);
  gimp_ellipse_select($img, $x - $r + $w, $y - $r + $w,
		      $r * 2 - 2 * $w, $r * 2 - 2 * $w,
			1, 1, 0, 0);
  gimp_bucket_fill($drw, 0, 0, 100, 0, 0, 0, 0);
}

sub drawCircleFill {
 my ($img, $drw, $x, $y, $r) = @_;
  gimp_selection_none($img);
  gimp_ellipse_select($img, $x - $r, $y - $r, $r * 2, $r * 2,
			0, 1, 0, 0);
  gimp_bucket_fill($drw, 0, 0, 100, 0, 0, 0, 0);
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












