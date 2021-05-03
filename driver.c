#include "marcher.h" // Includes ImgUtils.h

/****************************** Weight Functions *****************************/

double similarColour(Image *im, int a, int b)
{
  Pixel col1 = getPixel(im, a), col2 = getPixel(im, b);
  double a1 = (col1.R - col2.R);
  double b1 = (col1.G - col2.G);
  double c1 = (col1.B - col2.B);
  double sumSq = (a1 * a1) + (b1 * b1) + (c1 * c1);
  return sqrt(sumSq) + 0.01;
}

double howWhite(Image *im, int a, int b)
{
  // Don't even use the current pixel's colur here... the weight
  // functions are arbitrary :)
  Pixel col2 = getPixel(im, b);
  double a1 = (255 - col2.R);
  double b1 = (255 - col2.G);
  double c1 = (255 - col2.B);
  double sumSq = (a1 * a1) + (b1 * b1) + (c1 * c1);
  return sqrt(sumSq / 100.0) + 0.01;
}

/****************************** Main Driver **********************************/

void usageAndExit()
{
  fprintf(stderr, "Usage: ./driver <image> mode\n");
  fprintf(stderr, "    mode: 1 - Weight function (1) similarColour\n");
  fprintf(stderr, "          2 - Weight function (2) howWhite\n");
  fprintf(stderr, "          3 - Weight function (3) allColourWeight\n");
  exit(1);
}

int main(int argc, char *argv[])
{
  // Handle command line args
  if (argc != 3)
    usageAndExit();
  int mode = atoi(argv[2]);
  if (mode < 1 || mode > 3)
    usageAndExit();

  Image *im = readPPMimage(argv[1]);
  if (im == NULL)
    exit(1);

  // Make space for output path
  int *path = calloc(sizeof(int), im->sx * im->sy);
  if (path == NULL)
  {
    fprintf(stderr, "Could not allocate space for path.\n");
    exit(1);
  }

  // Call findPath with right weight function
  switch (mode)
  {
  case 1:
    findPath(im, similarColour, path);
    break;
  case 2:
    findPath(im, howWhite, path);
    break;
  case 3:
    findPath(im, allColourWeight, path);
    break;

  default:
    break;
  }

  // Output the image.
  outputPath(path, im);

  return 0;
}
