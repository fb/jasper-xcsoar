#include "RasterTile.h"
#include <stdlib.h>
#include <stdio.h>

#define min(x, y) \
        (((x) < (y)) ? (x) : (y))

/* Compute the maximum of two values. */
#define max(x, y) \
        (((x) > (y)) ? (x) : (y))


void RasterTile::Disable() {
  if (ImageBuffer) {
    free(ImageBuffer);
    ImageBuffer = NULL;    
  }
}

void RasterTile::Enable() {
  ImageBuffer = (short*)malloc(width*height*sizeof(short));    
}

bool RasterTile::IsEnabled() {
  if (!ImageBuffer || !width || !height) return false;
  return true;
}

short* RasterTile::GetImageBuffer() {
  return ImageBuffer;
}

short RasterTile::GetTerrainHeight(int x, int y) {
  if (!IsEnabled()) return -1;
  if ((x>xend)||(x<xstart)||(y>yend)||(y<ystart)) {
    return -1;
  }
  return ImageBuffer[(y-ystart)*width+(x-xstart)];
}


int RasterTile::CheckTileVisibility(int view_x, int view_y) {
  int dx1 = abs(view_x - xstart);
  int dx2 = abs(xend - view_x);
  int dy1 = abs(view_y - ystart);
  int dy2 = abs(yend - view_y);

  if (!width || !height) {
    if (IsEnabled()) {
      Disable();
    }
    return 0;
  }
  
  //  printf("%d,%d - %d,%d  wid %d hei %d dx %d,%d dy %d,%d\n", xstart, ystart, xend, yend, width, height, dx1, dx2, dy1, dy2);
  
  if (min(dx1,dx2)*2 < width*3) {
    if (min(dy1,dy2) < height) {
      return 1;
    }
  } 
  if (min(dy1,dy2)*2 < height*3) {
    if (min(dx1,dx2) < width) {
      return 1;
    }
  } 
  if ( (max(dx1,dx2) > width*2)
       || (max(dy1,dy2) > height*2)) {
    if (IsEnabled()) {
      Disable();
    }
    return 0;
  } 
  return 0;
}


bool RasterTile::VisibilityChanged(int view_x, int view_y) {
  if (CheckTileVisibility(view_x, view_y) && !IsEnabled()) {
    request = true;
  } else {
    request = false;
  }
  return request;
}


short* RasterTileCache::GetImageBuffer(int index) {
  return tiles[index].GetImageBuffer();
}


void RasterTileCache::SetTile(int i, int xstart, int ystart, int xend, int yend) {
  tiles[i].xstart = xstart;
  tiles[i].ystart = ystart;
  tiles[i].xend = xend;
  tiles[i].yend = yend;
  tiles[i].width = tiles[i].xend-tiles[i].xstart;
  tiles[i].height = tiles[i].yend-tiles[i].ystart;
}

bool RasterTileCache::PollTiles(int x, int y) {
  bool retval = false;
  view_x = x;
  view_y = y;
  for (int i=0; i<MAX_RTC_TILES; i++) {
    retval |= tiles[i].VisibilityChanged(view_x, view_y);
  }
  return retval;
}


bool RasterTileCache::TileRequest(int index) {
  bool retval = 0;
  int num_used = 0;

  retval = tiles[index].request;
  if (!retval) return false;

  for (int i=0; i< MAX_RTC_TILES; i++) {
    if (tiles[i].IsEnabled()) {
      num_used++;
    }
  }
  
  if (num_used< MAX_ACTIVE_TILES) {
    tiles[index].Enable();
    return true; // want to load this one!
  } else {
    return false; // not enough memory for it or not visible anyway
  }
};


short RasterTileCache::GetTerrainHeight(int x, int y) {
  int i;
  short retval;
  
  // search starting from last found tile
  i = tile_last;
  
  do {
    retval = tiles[i].GetTerrainHeight(x, y);
    if (retval>=0) {
      tile_last = i;
      return retval;
    }
    i++;
    if (i>= MAX_RTC_TILES) {
      i=0; // go back to start
    }
  } while (i != tile_last);
  
  return -1;
};


void RasterTileCache::SetSize(int _width, int _height) {
  width = _width;
  height = _height;
}


void RasterTileCache::SetLatLonBounds(double _lon_min, double _lon_max, double _lat_min, double _lat_max) {
  lon_min = _lon_min;
  lat_min = _lat_min;
  lon_max = _lon_max;
  lat_max = _lat_max;
}


void RasterTileCache::Reset() {
  tile_last = 0;
  view_x = 0;
  view_y = 0;
  width = 0;
  height = 0;
  initialised = false;
}


void RasterTileCache::SetInitialised(bool val) {
  if (!initialised && val) {
    if (lon_max-lon_min<0) {
      return;
    }
    if (lat_max-lat_min<0) {
      return;
    }
    initialised = true;
    return;
  }
  initialised = val;
}


bool RasterTileCache::GetInitialised(void) {
  return initialised;
}

