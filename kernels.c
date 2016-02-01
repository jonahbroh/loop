/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/*
 * Please fill in the following team struct
 */
team_t team = {
    "team 11",              /* Team name */

    "Aidan Q. Hendrickson",     /* First member full name */
    "",  /* First member email address */

    "Jonah P. Broh",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/*
 * naive_rotate - The naive baseline version of rotate
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst)
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/*
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst)
{
  int i, j, k, l;
  int s = dim/32;              //will split it into 32s
  for (k = 0; k<s; k++)         //for each fragment
  {
    int xs = k * 32;      //horizontal start
    int xe = xs + 32;     // horizontal end
    for (l = 0; l<s; l++)
    {
      int ys = l * 32;      //vertical start
      int ye = ys + 32;     //vertical end

      for (i = xs; i < (xe); i++)    //do the replacing within the fragment
        for (j = ys; j < ye; j++)
          dst[((dim-1-i)*(dim)+(j))] = src[((j)*(dim)+(i))];
    }
  }
}

char break_rotate_descr[] = "break rotate: breaks the picture 16 times each way";
void break_rotate(int dim, pixel *src, pixel *dst)
{                                   //split the operation into 16
  int i, j, k, l;
  int s = 16;              //how many splits?
  int f = dim/s;
  for (k = 0; k<s; k++)
  {
    int xs = k * f;      //where to start and end
    int xe = (k+1) * f;
    for (l = 0; l<s; l++)
    {
      int vs = l * f;
      int ve = (l+1)*f;

      for (i = xs; i < (xe); i++)    //do the swapping
        for (j = vs; j < ve; j++)
          dst[((dim-1-i)*(dim)+(j))] = src[((j)*(dim)+(i))];
    }
  }
}
char break2_rotate_descr[] = "break rotate: breaks the picture 4 times each way";
void break2_rotate(int dim, pixel *src, pixel *dst)
{                                   //split the operation into 4
  int i, j, k, l;
  int s = 4;              //how many splits?
  int f = dim/s;
  for (k = 0; k<s; k++)
  {
    int xs = k * f;      //where to start and end
    int xe = (k+1) * f;
    for (l = 0; l<s; l++)
    {
      int vs = l * f;
      int ve = (l+1)*f;

      for (i = xs; i < (xe); i++)    //do the swapping
        for (j = vs; j < ve; j++)
          dst[((dim-1-i)*(dim)+(j))] = src[((j)*(dim)+(i))];
    }
  }
}
char autobreak_rotate_descr[] = "autobreak rotate: breaks the picture into chunks of 32";
void autobreak_rotate(int dim, pixel *src, pixel *dst)
{
  int i, j, k, l;
  int s = dim/32;              //will split it into 32s
  for (k = 0; k<s; k++)         //for each fragment
  {
    int xs = k * 32;      //horizontal start
    int xe = xs + 32;     // horizontal end
    for (l = 0; l<s; l++)
    {
      int ys = l * 32;      //vertical start
      int ye = ys + 32;     //vertical end

      for (i = xs; i < (xe); i++)    //do the replacing within the fragment
        for (j = ys; j < ye; j++)
          dst[((dim-1-i)*(dim)+(j))] = src[((j)*(dim)+(i))];
    }
  }
}
char autobreak2_rotate_descr[] = "autobreak rotate: breaks the picture into 64s";
void autobreak2_rotate(int dim, pixel *src, pixel *dst)
{                                   //split the operation into 16
  int i, j, k, l;
  int s = dim/64;              //how many splits? - experiment with 64 vs 32
  int f = dim/s;
  for (k = 0; k<s; k++)
  {
    int xs = k * f;      //where to start and end
    int xe = (k+1) * f;
    for (l = 0; l<s; l++)
    {
      int vs = l * f;
      int ve = (l+1)*f;

      for (i = xs; i < (xe); i++)    //do the swapping
        for (j = vs; j < ve; j++)
          dst[((dim-1-i)*(dim)+(j))] = src[((j)*(dim)+(i))];
    }
  }
}
/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_rotate_functions()
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);
    add_rotate_function(&rotate, rotate_descr);
    /* ... Register additional test functions here */
    add_rotate_function(&break_rotate, break_rotate_descr);
    add_rotate_function(&break2_rotate, break2_rotate_descr);
    add_rotate_function(&autobreak_rotate, autobreak_rotate_descr);
    add_rotate_function(&autobreak2_rotate, autobreak2_rotate_descr);

}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
static void initialize_pixel_sum(pixel_sum *sum)
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/*
 * accumulate_sum - Accumulates field values of p in corresponding
 * fields of sum
 */
static void accumulate_sum(pixel_sum *sum, pixel p)
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/*
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum)
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/*
 * avg - Returns averaged pixel value at (i,j)
 */
static pixel avg(int dim, int i, int j, pixel *src)
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++)
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++)
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth
 */
 char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
 void naive_smooth(int dim, pixel *src, pixel *dst)
 {
     int i, j;

     for (i = 0; i < dim; i++)
 	for (j = 0; j < dim; j++)
 	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
 }

 /*
  * smooth - Your current working version of smooth.
  * IMPORTANT: This is the version you will be graded on
  */

  char smooth_descr[] = "smooth: Current working version";
  void smooth(int dim, pixel *src, pixel *dst)
  {
    int i, j, jp, ii, jj;
    pixel_sum sum;
    pixel current_pixel;
    for (i = 1; i < dim-1; i++)
      for (j = 1; j < dim-1; j+=2)
      {
        jp = j+1;

        //average funtion

        (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
        for(ii = i-1; ii <= i+1; ii++){
       for(jj = j-1; jj <= j+1; jj++)
         {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}

        (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
        dst[RIDX(i, j, dim)] = current_pixel;

       //average function 2
        (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
        for(ii = i-1; ii <= i+1; ii++){
       for(jj = jp-1; jj <= jp+1; jj++)
         {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}

        (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
        dst[RIDX(i, jp, dim)] = current_pixel;
      }
   for (i = 1; i < dim-1; i++)
   {
     //average function for top
     (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
     for(ii = i-1; ii <= i+1; ii++){
    for(jj = 0; jj <= 1; jj++)
      {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}

     (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
     dst[RIDX(i, 0, dim)] = current_pixel;
     // for bottom
     (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
     for(ii = i-1; ii <= i+1; ii++){
    for(jj = dim-2; jj <= dim-1; jj++)
      {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}

     (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
     dst[RIDX(i, dim-1, dim)] = current_pixel;

   }
   for (j = 1; j < dim-1; j++)
   {
     //for sides
     (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
     for(ii = 0; ii <= 1; ii++){
    for(jj = j-1; jj <= j+1; jj++)
      {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}
     (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
     dst[RIDX(0, j, dim)] = current_pixel;

     (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
     for(ii = dim-2; ii <= dim-1; ii++){
    for(jj = j-1; jj <= j+1; jj++)
      {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}
     (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
     dst[RIDX(dim-1, j, dim)] = current_pixel;
   }
   //for corners
   (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
   for(ii = 0; ii <= 1; ii++){
  for(jj = 0; jj <= 1; jj++)
    {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}
   (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
   dst[RIDX(0, 0, dim)] = current_pixel;

   (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
   for(ii = 0; ii <= 1; ii++){
  for(jj = dim-2; jj <= dim-1; jj++)
    {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}
   (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
   dst[RIDX(0, dim-1, dim)] = current_pixel;

   (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
   for(ii = dim-2; ii <= dim-1; ii++){
  for(jj = 0; jj <= 1; jj++)
    {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}
   (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
   dst[RIDX(dim-1, 0, dim)] = current_pixel;

   (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
   for(ii = dim-2; ii <= dim-1; ii++){
  for(jj = dim-2; jj <= dim-1; jj++)
    {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}
   (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
   dst[RIDX(dim-1, dim-1, dim)] = current_pixel;
  }
//   char inline_smooth_descr[] = "inline smooth: inlines average function, also unrolls loop";
//   void inline_smooth(int dim, pixel *src, pixel *dst)
//   {
//     int i, j, jp;
//     for (i = 0; i < dim; i++)
//       for (j = 0; j < dim; j+=2)
//       {
//         jp = j+1;
//
//         //average funtion
//         int ii, jj;
//         pixel_sum sum;
//         pixel current_pixel;
//
//         (&sum)->red = (&sum)->green = (&sum)->blue = 0;
// (&sum)->num = 0;
//         for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++)
//        for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++)
//          {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
// (&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
// (&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
// (&sum)->num++;}
//
//         assign_sum_to_pixel(&current_pixel, sum);
//         dst[RIDX(i, j, dim)] = current_pixel;
//
//        //average function 2
//         (&sum)->red = (&sum)->green = (&sum)->blue = 0;
// (&sum)->num = 0;
//         for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++)
//        for(jj = max(jp-1, 0); jj <= min(jp+1, dim-1); jj++)
//          {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
// (&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
// (&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
// (&sum)->num++;}
//
//         assign_sum_to_pixel(&current_pixel, sum);
//         dst[RIDX(i, jp, dim)] = current_pixel;
//       }
//   }


  char middle_smooth_descr[] = "middle smooth: does the middle seperate, also inlines average function, also unrolls loop";
  void middle_smooth(int dim, pixel *src, pixel *dst)
  {
    int i, j, jp, ii, jj;
    pixel_sum sum;
    pixel current_pixel;
    for (i = 1; i < dim-1; i++)
      for (j = 1; j < dim-1; j+=2)
      {
        jp = j+1;

        //average funtion

        (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
        for(ii = i-1; ii <= i+1; ii++){
       for(jj = j-1; jj <= j+1; jj++)
         {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}

        (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
        dst[RIDX(i, j, dim)] = current_pixel;

       //average function 2
        (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
        for(ii = i-1; ii <= i+1; ii++){
       for(jj = jp-1; jj <= jp+1; jj++)
         {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}

        (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
        dst[RIDX(i, jp, dim)] = current_pixel;
      }
   for (i = 1; i < dim-1; i++)
   {
     //average function for top
     (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
     for(ii = i-1; ii <= i+1; ii++){
    for(jj = 0; jj <= 1; jj++)
      {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}

     (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
     dst[RIDX(i, 0, dim)] = current_pixel;
     // for bottom
     (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
     for(ii = i-1; ii <= i+1; ii++){
    for(jj = dim-2; jj <= dim-1; jj++)
      {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}

     (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
     dst[RIDX(i, dim-1, dim)] = current_pixel;

   }
   for (j = 1; j < dim-1; j++)
   {
     //for sides
     (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
     for(ii = 0; ii <= 1; ii++){
    for(jj = j-1; jj <= j+1; jj++)
      {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}
     (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
     dst[RIDX(0, j, dim)] = current_pixel;

     (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
     for(ii = dim-2; ii <= dim-1; ii++){
    for(jj = j-1; jj <= j+1; jj++)
      {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}
     (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
     dst[RIDX(dim-1, j, dim)] = current_pixel;
   }
   //for corners
   (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
   for(ii = 0; ii <= 1; ii++){
  for(jj = 0; jj <= 1; jj++)
    {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}
   (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
   dst[RIDX(0, 0, dim)] = current_pixel;

   (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
   for(ii = 0; ii <= 1; ii++){
  for(jj = dim-2; jj <= dim-1; jj++)
    {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}
   (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
   dst[RIDX(0, dim-1, dim)] = current_pixel;

   (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
   for(ii = dim-2; ii <= dim-1; ii++){
  for(jj = 0; jj <= 1; jj++)
    {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}
   (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
   dst[RIDX(dim-1, 0, dim)] = current_pixel;

   (&sum)->red = (&sum)->green = (&sum)->blue = 0;
(&sum)->num = 0;
   for(ii = dim-2; ii <= dim-1; ii++){
  for(jj = dim-2; jj <= dim-1; jj++)
    {(&sum)->red += (int) src[RIDX(ii, jj, dim)].red;
(&sum)->green += (int) src[RIDX(ii, jj, dim)].green;
(&sum)->blue += (int) src[RIDX(ii, jj, dim)].blue;
(&sum)->num++;}}
   (&current_pixel)->red = (unsigned short) (sum.red/sum.num);
(&current_pixel)->green = (unsigned short) (sum.green/sum.num);
(&current_pixel)->blue = (unsigned short) (sum.blue/sum.num);
   dst[RIDX(dim-1, dim-1, dim)] = current_pixel;
  }


  char exp_smooth_descr[] = "exp_smooth: does something diferent idk";
  void exp_smooth(int dim, pixel *src, pixel *dst)
  {
      int i, j, ii;

      for (i = 0; i < 2; i++)
      {
        for (ii = 0 ; ii < dim; ii++)
        {
    	     for (j = 0; j < dim; j++)
           {
    	        dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
           }
         }
       }
  }
  //one loop for interior of image, no bounds checking
 char unravel_smooth_descr[] = "smooth unravel: unravels by amount u(2)";
 void unravel_smooth(int dim, pixel *src, pixel *dst)
 {
   int i, j, k, jp;
   int u = 2;

   for (i = 0; i < dim; i++)
     for (j = 0; j < dim; j+=2)
     {
       for (k = 0; k < u; k++)
       {
         jp = j+k;
         dst[RIDX(i, jp, dim)] = avg(dim, i, jp, src);
       }
     }
 }

 char unravel2_smooth_descr[] = "smooth unravel2: unravels by amount u(4)";
 void unravel2_smooth(int dim, pixel *src, pixel *dst)
 {
   int i, j, k, jp;
   int u = 4;

   for (i = 0; i < dim; i++)
     for (j = 0; j < dim; j+=2)
     {
       for (k = 0; k < u; k++)
       {
         jp = j+k;
         dst[RIDX(i, jp, dim)] = avg(dim, i, jp, src);
       }
     }
 }




 /*********************************************************************
  * register_smooth_functions - Register all of your different versions
  *     of the smooth kernel with the driver by calling the
  *     add_smooth_function() for each test function.  When you run the
  *     driver program, it will test and report the performance of each
  *     registered test function.
  *********************************************************************/

 void register_smooth_functions() {
     add_smooth_function(&naive_smooth, naive_smooth_descr);
     add_smooth_function(&smooth, smooth_descr);
     /* ... Register additional test functions here */
     add_smooth_function(&unravel_smooth, unravel_smooth_descr);
    //  add_smooth_function(inline_smooth, inline_smooth_descr);
     add_smooth_function(middle_smooth, middle_smooth_descr);
     //add_smooth_function(&unravel2_smooth, unravel2_smooth_descr);
 }
