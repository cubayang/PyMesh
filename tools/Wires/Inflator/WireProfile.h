#pragma once

#include <memory>
#include <string>
#include <Core/EigenTypedef.h>

class WireProfile {
    public:
        typedef std::shared_ptr<WireProfile> Ptr;
        static Ptr create(const std::string& name);
        static Ptr create_isotropic(size_t num_samples);
        static Ptr create_2D();

        virtual ~WireProfile() {}

    public:
        /**
         * Loop is a sequence of vertices around the Z axis in 3D or Y axis
         * in 2D.
         */
        void initialize(const MatrixFr& loop);

        /**
         * Place the edge loop along the edge (end_1, end_2) that is offseted
         * from end_1.  Thickness parameter specifies the diameter of the
         * bounding sphere of the loop.
         */
        MatrixFr place(const VectorF& end_1, const VectorF& end_2,
                Float offset, Float thickness);

        size_t size() const { return m_loop.rows(); }

    protected:
        size_t m_dim;
        MatrixFr m_loop;
        Float m_radius;
        VectorF m_offset_dir;
};