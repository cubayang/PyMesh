/* This file is part of PyMesh. Copyright (c) 2017 by Qingnan Zhou */
#include "VoxelCircumCenterAttribute.h"

#include <Mesh.h>
#include <Core/Exception.h>

using namespace PyMesh;

void VoxelCircumCenterAttribute::compute_from_mesh(Mesh& mesh) {
    const size_t dim = mesh.get_dim();
    const size_t num_voxels = mesh.get_num_voxels();
    const size_t vertex_per_voxel = mesh.get_vertex_per_voxel();

    if (vertex_per_voxel != 4) {
        throw NotImplementedError(
                "Voxel circumcenter is defined for 3D tet mesh only.");
    }
    if (dim != 3) {
        throw RuntimeError("Voxel circumcenter is defined for 3D tet mesh only.");
    }

    VectorF& circumcenter = m_values;
    circumcenter.resize(num_voxels * 3);
    const auto& voxels = mesh.get_voxels();
    const auto& vertices = mesh.get_vertices();

    for (size_t i=0; i<num_voxels; i++) {
        Vector4I voxel = voxels.segment<4>(i*4);
        Vector3F v0 = vertices.segment<3>(voxel[0]*3);
        Vector3F v1 = vertices.segment<3>(voxel[1]*3);
        Vector3F v2 = vertices.segment<3>(voxel[2]*3);
        Vector3F v3 = vertices.segment<3>(voxel[3]*3);

        Float v0_norm = v0.norm();
        Float v1_norm = v1.norm();
        Float v2_norm = v2.norm();
        Float v3_norm = v3.norm();

        Matrix4F alpha;
        alpha.row(0) << v0, 1;
        alpha.row(1) << v1, 1;
        alpha.row(2) << v2, 1;
        alpha.row(3) << v3, 1;
        Float alpha_det = alpha.determinant();

        Matrix4F Dx;
        Dx.row(0) << v0_norm, v0[1], v0[2], 1.0;
        Dx.row(1) << v1_norm, v1[1], v1[2], 1.0;
        Dx.row(2) << v2_norm, v2[1], v2[2], 1.0;
        Dx.row(3) << v3_norm, v3[1], v3[2], 1.0;
        Float Dx_det = Dx.determinant();

        Matrix4F Dy;
        Dy.row(0) << v0_norm, v0[0], v0[2], 1.0;
        Dy.row(1) << v1_norm, v1[0], v1[2], 1.0;
        Dy.row(2) << v2_norm, v2[0], v2[2], 1.0;
        Dy.row(3) << v3_norm, v3[0], v3[2], 1.0;
        Float Dy_det = -Dy.determinant();

        Matrix4F Dz;
        Dz.row(0) << v0_norm, v0[0], v0[1], 1.0;
        Dz.row(1) << v1_norm, v1[0], v1[1], 1.0;
        Dz.row(2) << v2_norm, v2[0], v2[1], 1.0;
        Dz.row(3) << v3_norm, v3[0], v3[1], 1.0;
        Float Dz_det = Dz.determinant();

        circumcenter.segment<3>(i*3) <<
            Dx_det / (2 * alpha_det),
            Dy_det / (2 * alpha_det),
            Dz_det / (2 * alpha_det);
    }
}
