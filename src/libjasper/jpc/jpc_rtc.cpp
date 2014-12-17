#include "jpc_rtc.h"
#include "RasterTile.h"

RasterTileCache jas_rtc;

extern "C" {

  void jas_rtc_SetTile(int index, int xstart, int ystart, int xend, int yend) {
    jas_rtc.SetTile(index, xstart, ystart, xend, yend);
  }
  
  bool jas_rtc_TileRequest(int index) {
    return jas_rtc.TileRequest(index);
  }

  bool jas_rtc_PollTiles(int view_x, int view_y) {
    return jas_rtc.PollTiles(view_x, view_y);
  }
  
  short* jas_rtc_GetImageBuffer(int index) {
    return jas_rtc.GetImageBuffer(index);
  }

  void jas_rtc_SetLatLonBounds(double lon_min, double lon_max, double lat_min, double lat_max) {
    jas_rtc.SetLatLonBounds(lon_min, lon_max, lat_min, lat_max);
  }

  void jas_rtc_SetSize(int width, int height) {
    jas_rtc.SetSize(width, height);
  }

  void jas_rtc_SetInitialised(bool val) {
    jas_rtc.SetInitialised(val);
  }

  bool jas_rtc_GetInitialised(void) {
    return jas_rtc.GetInitialised();
  }

}
