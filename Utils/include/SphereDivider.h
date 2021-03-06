/*
###################################################################################
#
# BCMTools
#
# Copyright (c) 2011-2014 Institute of Industrial Science, The University of Tokyo.
# All rights reserved.
#
# Copyright (c) 2012-2016 Advanced Institute for Computational Science (AICS), RIKEN.
# All rights reserved.
#
# Copyright (c) 2017 Research Institute for Information Technology (RIIT), Kyushu University.
# All rights reserved.
#
###################################################################################
*/

///
/// @file SphereDivider.h
/// @brief ブロック分割判定クラス(SphereDivider)
///

#ifndef SPHERE_DIVIDER_H
#define SPHERE_DIVIDER_H

#include <cmath>
#include <algorithm> // for max, min
#include "BCMTools.h"
#include "MultiRootDivider.h"
#include "BoundingBox.h"

/// 球面外部をブロック分割するDivider.
///
/// ルートブロックの大きさと位置は固定
///   - 大きさ: 単位立方体
///   - 位置: 最初のルートブロックの原点が(0,0,0)
///
class SphereDivider : public MultiRootDivider {

  const int minLevel;  ///< 最小分割レベル
  const int maxLevel;  ///< 最大分割レベル

  const REAL_TYPE ox;  ///< 球中心座標
  const REAL_TYPE oy;  ///< 球中心座標
  const REAL_TYPE oz;  ///< 球中心座標
  const REAL_TYPE r;   ///< 球半径

  const REAL_TYPE r2;  ///< r^2

  Vec3r margin;  ///< 追加マージン

public:

  /// コンストラクタ.
  ///
  ///  @param[in] rootGrid ルートブロック配置情報
  ///  @param[in] minLevel 最小分割レベル
  ///  @param[in] maxLevel 最大分割レベル
  ///  @param[in] ox,oy,oz 球中心座標
  ///  @param[in] r        球半径
  ///  @param[in] extraMarginRatio 追加マージンの幅
  ///
  ///  @note 境界探査領域にマージンを追加する場合には，
  ///  追加マージン幅の最大分割レベルブロック辺長に対する比率を
  ///  extraMarginRatioに指定する．
  ///  例えば，仮想セル領域を境界面探査領域に追加するには
  ///  「(REAL_TYPE)仮想セル数/ブロック内分割数」を指定する．
  ///
  SphereDivider(const RootGrid* rootGrid, int minLevel, int maxLevel,
                REAL_TYPE ox, REAL_TYPE oy, REAL_TYPE oz, REAL_TYPE r,
                REAL_TYPE extraMarginRatio = 0.0)
    : MultiRootDivider(Vec3r(0.0,0.0,0.0), 1.0, rootGrid),
      minLevel(minLevel), maxLevel(maxLevel),
      ox(ox), oy(oy), oz(oz), r(r), r2(r*r) {
    assert(minLevel >= 0);
    assert(maxLevel >= minLevel);

    margin = Vec3r(extraMarginRatio / (1 << maxLevel));
  }

  /// デストラクタ.
  ~SphereDivider() {}

  /// ブロックを分割するかどうかを判定.
  ///
  ///   @param[in] pedigree ブロックのPedigree
  ///   @return ブロックタイプ
  ///
  NodeType operator() (const Pedigree& pedigree) {
    int level = pedigree.getLevel();

    if  (level < minLevel) return BRANCH;

    BoundingBox region = defineSearchRegion(pedigree, maxLevel);

    Vec3r min = region.getMin() - margin;
    Vec3r max = region.getMax() + margin;

  //std::cout << "(x0,y0,z0) = " << min << std::endl;
  //std::cout << "(x1,y1,z1) = " << max << std::endl;

    if (checkInner(min.x, min.y, min.z, max.x, max.y, max.z)) return LEAF_NO_ACTIVE;

    if (checkOuter(min.x, min.y, min.z, max.x, max.y, max.z)) return LEAF_ACTIVE;

    if  (level == maxLevel) return LEAF_ACTIVE;

    return BRANCH;
  }


private:

  /// 矩形領域が球面内に完全に含まれるかどうかチェック.
  ///
  /// 矩形領域内で球中心から最も遠い点を求め，その距離と球半径を比較する.
  ///
  bool checkInner(REAL_TYPE x0, REAL_TYPE y0, REAL_TYPE z0,
                  REAL_TYPE x1, REAL_TYPE y1, REAL_TYPE z1) {

    REAL_TYPE x = std::max(fabs(x0-ox), fabs(x1-ox));
    REAL_TYPE r2max = x * x;
    if (r2max >= r2) return false;

    REAL_TYPE y = std::max(fabs(y0-oy), fabs(y1-oy));
    r2max += y * y;
    if (r2max >= r2) return false;

    REAL_TYPE z = std::max(fabs(z0-oz), fabs(z1-oz));
    r2max += z * z;
    if (r2max >= r2) return false;

    return true;
  }


  /// 矩形領域が球面の外部にあるかどうかチェック.
  ///
  /// 矩形領域内で球中心に最も近い点を求め，その距離と球半径を比較する.
  ///
  bool checkOuter(REAL_TYPE x0, REAL_TYPE y0, REAL_TYPE z0,
                  REAL_TYPE x1, REAL_TYPE y1, REAL_TYPE z1) {

    REAL_TYPE x = 0.0;
    if      (ox < x0) x = x0 - ox;
    else if (ox > x1) x = ox - x1;
    REAL_TYPE r2min = x * x;
    if (r2min >= r2) return true;

    REAL_TYPE y = 0.0;
    if      (oy < y0) y = y0 - oy;
    else if (oy > y1) y = oy - y1;
    r2min += y * y;
    if (r2min >= r2) return true;

    REAL_TYPE z = 0.0;
    if      (oz < z0) z = z0 - oz;
    else if (oz > z1) z = oz - z1;
    r2min += z * z;
    if (r2min >= r2) return true;

    return false;
  }

};



#endif // SPHERE_DIVIDER_H
