#ifndef RASTERTILE_H
#define RASTERTILE_H

class RasterTile {
public:
  RasterTile() {
    xstart = ystart = xend = yend = 0;
    width = height = 0;
  }

  int xstart, ystart, xend, yend;
  int width, height;
  bool request;
  short *ImageBuffer;

  int CheckTileVisibility(int view_x, int view_y);

 public:
  void Disable();
  void Enable();
  bool IsEnabled();
  short GetTerrainHeight(int x, int y); 
  short* GetImageBuffer();
  bool VisibilityChanged(int view_x, int view_y);
};

#define MAX_RTC_TILES 1024
#define MAX_ACTIVE_TILES 16

class RasterTileCache {
public:

  RasterTileCache() { 
    Reset();
  };

private:
  int view_x;
  int view_y;
  bool initialised;
private:  
  RasterTile tiles[MAX_RTC_TILES];
  int tile_last;

public:
  double lat_min, lat_max, lon_min, lon_max;
  int width, height;
  // public only for debugging!

  void Reset();
  short* GetImageBuffer(int index);
  bool PollTiles(int x, int y);
  void SetTile(int index, int xstart, int ystart, int xend, int yend);
  bool TileRequest(int index);

  short GetTerrainHeight(int x, int y);
  void SetLatLonBounds(double lon_min, double lon_max, double lat_min, double lat_max);
  void SetSize(int width, int height);
  void SetInitialised(bool val);
  bool GetInitialised(void);
};

#endif
