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
/// @file BlockFactory.h
/// @brief ブロックファクトリクラス
///

#ifndef BLOCK_FACTORY_H
#define BLOCK_FACTORY_H

#include "Block.h"
#include "BCMOctree.h"
#include "Partition.h"
#include "BoundaryConditionSetterBase.h"


/// ブロックファクトリクラス.
///
/// コンストラクタに渡すBoundaryConditionSetterの継承クラスにより，
/// 境界条件をカスタマイズできる.
///
class BlockFactory {

  const BCMOctree* tree;   ///< Octreeデータ

  const Partition* partition;  ///< 領域分割情報

  BoundaryConditionSetterBase* boundaryConditionSetter;  ///< 境界条件設定クラス

//const Vec3r& rootOrigin;  ///< ID=0のルートブロックの原点位置
  const Vec3r rootOrigin;  ///< ID=0のルートブロックの原点位置

  REAL_TYPE rootLength;  ///< ルートブロックの辺長

  const Vec3i& size;  ///< ブロック内分割数

public:

  /// コンストラクタ.
  ///
  ///  @param[in] tree Octreeデータ
  ///  @param[in] partition 領域分割情報
  ///  @param[in] boundaryConditionSetter 境界条件設定クラス
  ///  @param[in] rootOrigin ID=0のルートブロックの原点位置
  ///  @param[in] rootLength ルートブロックの辺長
  ///  @param[in] size ブロック内分割数
  ///
  BlockFactory(const BCMOctree* tree, const Partition* partition,
               BoundaryConditionSetterBase* boundaryConditionSetter,
               const Vec3r& rootOrigin, REAL_TYPE rootLength, const Vec3i& size)
    : tree(tree), partition(partition),
      boundaryConditionSetter(boundaryConditionSetter),
      rootOrigin(rootOrigin), rootLength(rootLength), size(size)
  {}

  /// コンストラクタ(ルートブロックは単位立方体).
  ///
  ///  @param[in] tree Octreeデータ
  ///  @param[in] partition 領域分割情報
  ///  @param[in] boundaryConditionSetter 境界条件設定クラス
  ///  @param[in] size ブロック内分割数
  ///
  BlockFactory(const BCMOctree* tree, const Partition* partition,
               BoundaryConditionSetterBase* boundaryConditionSetter, const Vec3i& size)
    : tree(tree), partition(partition),
      boundaryConditionSetter(boundaryConditionSetter),
      rootOrigin(0.0,0.0,0.0), rootLength(1.0), size(size)
  {}

  /// デストラクタ.
  ~BlockFactory() {}

  /// ブロック生成.
  Block* makeBlock(const Node* node) {
    Vec3r origin = tree->getOrigin(node) * rootLength + rootOrigin;
    Vec3r blockSize = node->getBlockSize() * rootLength;
    int level = node->getLevel();
    BoundaryInfo* boundaryInfo = (*boundaryConditionSetter)(node, tree);
    NeighborInfo* neighborInfo = tree->makeNeighborInfo(node, partition);
    return new Block(size, origin, blockSize, level, neighborInfo, boundaryInfo);
  }


};


#endif // BLOCK_FACTORY_H
