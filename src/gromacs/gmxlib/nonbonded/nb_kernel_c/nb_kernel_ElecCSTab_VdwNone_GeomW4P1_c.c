/*
 * This file is part of the GROMACS molecular simulation package.
 *
 * Copyright (c) 2012,2013,2014.2015, by the GROMACS development team, led by
 * Mark Abraham, David van der Spoel, Berk Hess, and Erik Lindahl,
 * and including many others, as listed in the AUTHORS file in the
 * top-level source directory and at http://www.gromacs.org.
 *
 * GROMACS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * GROMACS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GROMACS; if not, see
 * http://www.gnu.org/licenses, or write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
 *
 * If you want to redistribute modifications to GROMACS, please
 * consider that scientific software is very special. Version
 * control is crucial - bugs must be traceable. We will be happy to
 * consider code for inclusion in the official distribution, but
 * derived work must not be called official GROMACS. Details are found
 * in the README & COPYING files - if they are missing, get the
 * official version at http://www.gromacs.org.
 *
 * To help us fund GROMACS development, we humbly ask that you cite
 * the research papers on the package. Check out http://www.gromacs.org.
 */
/*
 * Note: this file was generated by the GROMACS c kernel generator.
 */
#include "gmxpre.h"
#include "config.h"
#include <math.h>

#include "../nb_kernel.h"
#include "gromacs/fda/fda.h"
#include "gromacs/gmxlib/nrnb.h"

/*
 * Gromacs nonbonded kernel:   nb_kernel_ElecCSTab_VdwNone_GeomW4P1_VF_c
 * Electrostatics interaction: CubicSplineTable
 * VdW interaction:            None
 * Geometry:                   Water4-Particle
 * Calculate force/pot:        PotentialAndForce
 */
void
nb_kernel_ElecCSTab_VdwNone_GeomW4P1_VF_c
                    (t_nblist                    * gmx_restrict       nlist,
                     rvec                        * gmx_restrict          xx,
                     rvec                        * gmx_restrict          ff,
                     t_forcerec                  * gmx_restrict          fr,
                     t_mdatoms                   * gmx_restrict     mdatoms,
                     nb_kernel_data_t gmx_unused * gmx_restrict kernel_data,
                     t_nrnb                      * gmx_restrict        nrnb)
{
    int              i_shift_offset,i_coord_offset,j_coord_offset;
    int              j_index_start,j_index_end;
    int              nri,inr,ggid,iidx,jidx,jnr,outeriter,inneriter;
    real             shX,shY,shZ,tx,ty,tz,fscal,rcutoff,rcutoff2;
    int              *iinr,*jindex,*jjnr,*shiftidx,*gid;
    real             *shiftvec,*fshift,*x,*f;
    int              vdwioffset1;
    real             ix1,iy1,iz1,fix1,fiy1,fiz1,iq1,isai1;
    int              vdwioffset2;
    real             ix2,iy2,iz2,fix2,fiy2,fiz2,iq2,isai2;
    int              vdwioffset3;
    real             ix3,iy3,iz3,fix3,fiy3,fiz3,iq3,isai3;
    int              vdwjidx0;
    real             jx0,jy0,jz0,fjx0,fjy0,fjz0,jq0,isaj0;
    real             dx10,dy10,dz10,rsq10,rinv10,rinvsq10,r10,qq10,c6_10,c12_10,cexp1_10,cexp2_10;
    real             dx20,dy20,dz20,rsq20,rinv20,rinvsq20,r20,qq20,c6_20,c12_20,cexp1_20,cexp2_20;
    real             dx30,dy30,dz30,rsq30,rinv30,rinvsq30,r30,qq30,c6_30,c12_30,cexp1_30,cexp2_30;
    real             velec,felec,velecsum,facel,crf,krf,krf2;
    real             *charge;
    int              vfitab;
    real             rt,vfeps,vftabscale,Y,F,Geps,Heps2,Fp,VV,FF;
    real             *vftab;

    struct FDA *fda = fr->fda;

    x                = xx[0];
    f                = ff[0];

    nri              = nlist->nri;
    iinr             = nlist->iinr;
    jindex           = nlist->jindex;
    jjnr             = nlist->jjnr;
    shiftidx         = nlist->shift;
    gid              = nlist->gid;
    shiftvec         = fr->shift_vec[0];
    fshift           = fr->fshift[0];
    facel            = fr->epsfac;
    charge           = mdatoms->chargeA;

    vftab            = kernel_data->table_elec->data;
    vftabscale       = kernel_data->table_elec->scale;

    /* Setup water-specific parameters */
    inr              = nlist->iinr[0];
    iq1              = facel*charge[inr+1];
    iq2              = facel*charge[inr+2];
    iq3              = facel*charge[inr+3];

    outeriter        = 0;
    inneriter        = 0;

    /* Start outer loop over neighborlists */
    for(iidx=0; iidx<nri; iidx++)
    {
        /* Load shift vector for this list */
        i_shift_offset   = DIM*shiftidx[iidx];
        shX              = shiftvec[i_shift_offset+XX];
        shY              = shiftvec[i_shift_offset+YY];
        shZ              = shiftvec[i_shift_offset+ZZ];

        /* Load limits for loop over neighbors */
        j_index_start    = jindex[iidx];
        j_index_end      = jindex[iidx+1];

        /* Get outer coordinate index */
        inr              = iinr[iidx];
        i_coord_offset   = DIM*inr;

        /* Load i particle coords and add shift vector */
        ix1              = shX + x[i_coord_offset+DIM*1+XX];
        iy1              = shY + x[i_coord_offset+DIM*1+YY];
        iz1              = shZ + x[i_coord_offset+DIM*1+ZZ];
        ix2              = shX + x[i_coord_offset+DIM*2+XX];
        iy2              = shY + x[i_coord_offset+DIM*2+YY];
        iz2              = shZ + x[i_coord_offset+DIM*2+ZZ];
        ix3              = shX + x[i_coord_offset+DIM*3+XX];
        iy3              = shY + x[i_coord_offset+DIM*3+YY];
        iz3              = shZ + x[i_coord_offset+DIM*3+ZZ];

        fix1             = 0.0;
        fiy1             = 0.0;
        fiz1             = 0.0;
        fix2             = 0.0;
        fiy2             = 0.0;
        fiz2             = 0.0;
        fix3             = 0.0;
        fiy3             = 0.0;
        fiz3             = 0.0;

        /* Reset potential sums */
        velecsum         = 0.0;

        /* Start inner kernel loop */
        for(jidx=j_index_start; jidx<j_index_end; jidx++)
        {
            /* Get j neighbor index, and coordinate index */
            jnr              = jjnr[jidx];
            j_coord_offset   = DIM*jnr;

            /* load j atom coordinates */
            jx0              = x[j_coord_offset+DIM*0+XX];
            jy0              = x[j_coord_offset+DIM*0+YY];
            jz0              = x[j_coord_offset+DIM*0+ZZ];

            /* Calculate displacement vector */
            dx10             = ix1 - jx0;
            dy10             = iy1 - jy0;
            dz10             = iz1 - jz0;
            dx20             = ix2 - jx0;
            dy20             = iy2 - jy0;
            dz20             = iz2 - jz0;
            dx30             = ix3 - jx0;
            dy30             = iy3 - jy0;
            dz30             = iz3 - jz0;

            /* Calculate squared distance and things based on it */
            rsq10            = dx10*dx10+dy10*dy10+dz10*dz10;
            rsq20            = dx20*dx20+dy20*dy20+dz20*dz20;
            rsq30            = dx30*dx30+dy30*dy30+dz30*dz30;

            rinv10           = 1.0/sqrt(rsq10);
            rinv20           = 1.0/sqrt(rsq20);
            rinv30           = 1.0/sqrt(rsq30);

            /* Load parameters for j particles */
            jq0              = charge[jnr+0];

            /**************************
             * CALCULATE INTERACTIONS *
             **************************/

            r10              = rsq10*rinv10;

            qq10             = iq1*jq0;

            /* Calculate table index by multiplying r with table scale and truncate to integer */
            rt               = r10*vftabscale;
            vfitab           = rt;
            vfeps            = rt-vfitab;
            vfitab           = 1*4*vfitab;

            /* CUBIC SPLINE TABLE ELECTROSTATICS */
            Y                = vftab[vfitab];
            F                = vftab[vfitab+1];
            Geps             = vfeps*vftab[vfitab+2];
            Heps2            = vfeps*vfeps*vftab[vfitab+3];
            Fp               = F+Geps+Heps2;
            VV               = Y+vfeps*Fp;
            velec            = qq10*VV;
            FF               = Fp+Geps+2.0*Heps2;
            felec            = -qq10*FF*vftabscale*rinv10;

            /* Update potential sums from outer loop */
            velecsum        += velec;

            fscal            = felec;

            /* Calculate temporary vectorial force */
            tx               = fscal*dx10;
            ty               = fscal*dy10;
            tz               = fscal*dz10;

            /* Update vectorial force */
            fix1            += tx;
            fiy1            += ty;
            fiz1            += tz;
            f[j_coord_offset+DIM*0+XX] -= tx;
            f[j_coord_offset+DIM*0+YY] -= ty;
            f[j_coord_offset+DIM*0+ZZ] -= tz;

            /* pairwise forces */
            fda_add_nonbonded_single(fda, inr+1, jnr+0, InteractionType_COULOMB, felec, dx10, dy10, dz10);
            fda_virial_bond(fda, inr+1, jnr+0, fscal, dx10, dy10, dz10);

            /**************************
             * CALCULATE INTERACTIONS *
             **************************/

            r20              = rsq20*rinv20;

            qq20             = iq2*jq0;

            /* Calculate table index by multiplying r with table scale and truncate to integer */
            rt               = r20*vftabscale;
            vfitab           = rt;
            vfeps            = rt-vfitab;
            vfitab           = 1*4*vfitab;

            /* CUBIC SPLINE TABLE ELECTROSTATICS */
            Y                = vftab[vfitab];
            F                = vftab[vfitab+1];
            Geps             = vfeps*vftab[vfitab+2];
            Heps2            = vfeps*vfeps*vftab[vfitab+3];
            Fp               = F+Geps+Heps2;
            VV               = Y+vfeps*Fp;
            velec            = qq20*VV;
            FF               = Fp+Geps+2.0*Heps2;
            felec            = -qq20*FF*vftabscale*rinv20;

            /* Update potential sums from outer loop */
            velecsum        += velec;

            fscal            = felec;

            /* Calculate temporary vectorial force */
            tx               = fscal*dx20;
            ty               = fscal*dy20;
            tz               = fscal*dz20;

            /* Update vectorial force */
            fix2            += tx;
            fiy2            += ty;
            fiz2            += tz;
            f[j_coord_offset+DIM*0+XX] -= tx;
            f[j_coord_offset+DIM*0+YY] -= ty;
            f[j_coord_offset+DIM*0+ZZ] -= tz;

            /* pairwise forces */
            fda_add_nonbonded_single(fda, inr+2, jnr+0, InteractionType_COULOMB, felec, dx20, dy20, dz20);
            fda_virial_bond(fda, inr+2, jnr+0, fscal, dx20, dy20, dz20);

            /**************************
             * CALCULATE INTERACTIONS *
             **************************/

            r30              = rsq30*rinv30;

            qq30             = iq3*jq0;

            /* Calculate table index by multiplying r with table scale and truncate to integer */
            rt               = r30*vftabscale;
            vfitab           = rt;
            vfeps            = rt-vfitab;
            vfitab           = 1*4*vfitab;

            /* CUBIC SPLINE TABLE ELECTROSTATICS */
            Y                = vftab[vfitab];
            F                = vftab[vfitab+1];
            Geps             = vfeps*vftab[vfitab+2];
            Heps2            = vfeps*vfeps*vftab[vfitab+3];
            Fp               = F+Geps+Heps2;
            VV               = Y+vfeps*Fp;
            velec            = qq30*VV;
            FF               = Fp+Geps+2.0*Heps2;
            felec            = -qq30*FF*vftabscale*rinv30;

            /* Update potential sums from outer loop */
            velecsum        += velec;

            fscal            = felec;

            /* Calculate temporary vectorial force */
            tx               = fscal*dx30;
            ty               = fscal*dy30;
            tz               = fscal*dz30;

            /* Update vectorial force */
            fix3            += tx;
            fiy3            += ty;
            fiz3            += tz;
            f[j_coord_offset+DIM*0+XX] -= tx;
            f[j_coord_offset+DIM*0+YY] -= ty;
            f[j_coord_offset+DIM*0+ZZ] -= tz;

            /* pairwise forces */
            fda_add_nonbonded_single(fda, inr+3, jnr+0, InteractionType_COULOMB, felec, dx30, dy30, dz30);
            fda_virial_bond(fda, inr+3, jnr+0, fscal, dx30, dy30, dz30);

            /* Inner loop uses 126 flops */
        }
        /* End of innermost loop */

        tx = ty = tz = 0;
        f[i_coord_offset+DIM*1+XX] += fix1;
        f[i_coord_offset+DIM*1+YY] += fiy1;
        f[i_coord_offset+DIM*1+ZZ] += fiz1;
        tx                         += fix1;
        ty                         += fiy1;
        tz                         += fiz1;
        f[i_coord_offset+DIM*2+XX] += fix2;
        f[i_coord_offset+DIM*2+YY] += fiy2;
        f[i_coord_offset+DIM*2+ZZ] += fiz2;
        tx                         += fix2;
        ty                         += fiy2;
        tz                         += fiz2;
        f[i_coord_offset+DIM*3+XX] += fix3;
        f[i_coord_offset+DIM*3+YY] += fiy3;
        f[i_coord_offset+DIM*3+ZZ] += fiz3;
        tx                         += fix3;
        ty                         += fiy3;
        tz                         += fiz3;
        fshift[i_shift_offset+XX]  += tx;
        fshift[i_shift_offset+YY]  += ty;
        fshift[i_shift_offset+ZZ]  += tz;

        ggid                        = gid[iidx];
        /* Update potential energies */
        kernel_data->energygrp_elec[ggid] += velecsum;

        /* Increment number of inner iterations */
        inneriter                  += j_index_end - j_index_start;

        /* Outer loop uses 31 flops */
    }

    /* Increment number of outer iterations */
    outeriter        += nri;

    /* Update outer/inner flops */

    inc_nrnb(nrnb,eNR_NBKERNEL_ELEC_W4_VF,outeriter*31 + inneriter*126);
}
/*
 * Gromacs nonbonded kernel:   nb_kernel_ElecCSTab_VdwNone_GeomW4P1_F_c
 * Electrostatics interaction: CubicSplineTable
 * VdW interaction:            None
 * Geometry:                   Water4-Particle
 * Calculate force/pot:        Force
 */
void
nb_kernel_ElecCSTab_VdwNone_GeomW4P1_F_c
                    (t_nblist                    * gmx_restrict       nlist,
                     rvec                        * gmx_restrict          xx,
                     rvec                        * gmx_restrict          ff,
                     t_forcerec                  * gmx_restrict          fr,
                     t_mdatoms                   * gmx_restrict     mdatoms,
                     nb_kernel_data_t gmx_unused * gmx_restrict kernel_data,
                     t_nrnb                      * gmx_restrict        nrnb)
{
    int              i_shift_offset,i_coord_offset,j_coord_offset;
    int              j_index_start,j_index_end;
    int              nri,inr,ggid,iidx,jidx,jnr,outeriter,inneriter;
    real             shX,shY,shZ,tx,ty,tz,fscal,rcutoff,rcutoff2;
    int              *iinr,*jindex,*jjnr,*shiftidx,*gid;
    real             *shiftvec,*fshift,*x,*f;
    int              vdwioffset1;
    real             ix1,iy1,iz1,fix1,fiy1,fiz1,iq1,isai1;
    int              vdwioffset2;
    real             ix2,iy2,iz2,fix2,fiy2,fiz2,iq2,isai2;
    int              vdwioffset3;
    real             ix3,iy3,iz3,fix3,fiy3,fiz3,iq3,isai3;
    int              vdwjidx0;
    real             jx0,jy0,jz0,fjx0,fjy0,fjz0,jq0,isaj0;
    real             dx10,dy10,dz10,rsq10,rinv10,rinvsq10,r10,qq10,c6_10,c12_10,cexp1_10,cexp2_10;
    real             dx20,dy20,dz20,rsq20,rinv20,rinvsq20,r20,qq20,c6_20,c12_20,cexp1_20,cexp2_20;
    real             dx30,dy30,dz30,rsq30,rinv30,rinvsq30,r30,qq30,c6_30,c12_30,cexp1_30,cexp2_30;
    real             velec,felec,velecsum,facel,crf,krf,krf2;
    real             *charge;
    int              vfitab;
    real             rt,vfeps,vftabscale,Y,F,Geps,Heps2,Fp,VV,FF;
    real             *vftab;

    struct FDA *fda = fr->fda;

    x                = xx[0];
    f                = ff[0];

    nri              = nlist->nri;
    iinr             = nlist->iinr;
    jindex           = nlist->jindex;
    jjnr             = nlist->jjnr;
    shiftidx         = nlist->shift;
    gid              = nlist->gid;
    shiftvec         = fr->shift_vec[0];
    fshift           = fr->fshift[0];
    facel            = fr->epsfac;
    charge           = mdatoms->chargeA;

    vftab            = kernel_data->table_elec->data;
    vftabscale       = kernel_data->table_elec->scale;

    /* Setup water-specific parameters */
    inr              = nlist->iinr[0];
    iq1              = facel*charge[inr+1];
    iq2              = facel*charge[inr+2];
    iq3              = facel*charge[inr+3];

    outeriter        = 0;
    inneriter        = 0;

    /* Start outer loop over neighborlists */
    for(iidx=0; iidx<nri; iidx++)
    {
        /* Load shift vector for this list */
        i_shift_offset   = DIM*shiftidx[iidx];
        shX              = shiftvec[i_shift_offset+XX];
        shY              = shiftvec[i_shift_offset+YY];
        shZ              = shiftvec[i_shift_offset+ZZ];

        /* Load limits for loop over neighbors */
        j_index_start    = jindex[iidx];
        j_index_end      = jindex[iidx+1];

        /* Get outer coordinate index */
        inr              = iinr[iidx];
        i_coord_offset   = DIM*inr;

        /* Load i particle coords and add shift vector */
        ix1              = shX + x[i_coord_offset+DIM*1+XX];
        iy1              = shY + x[i_coord_offset+DIM*1+YY];
        iz1              = shZ + x[i_coord_offset+DIM*1+ZZ];
        ix2              = shX + x[i_coord_offset+DIM*2+XX];
        iy2              = shY + x[i_coord_offset+DIM*2+YY];
        iz2              = shZ + x[i_coord_offset+DIM*2+ZZ];
        ix3              = shX + x[i_coord_offset+DIM*3+XX];
        iy3              = shY + x[i_coord_offset+DIM*3+YY];
        iz3              = shZ + x[i_coord_offset+DIM*3+ZZ];

        fix1             = 0.0;
        fiy1             = 0.0;
        fiz1             = 0.0;
        fix2             = 0.0;
        fiy2             = 0.0;
        fiz2             = 0.0;
        fix3             = 0.0;
        fiy3             = 0.0;
        fiz3             = 0.0;

        /* Start inner kernel loop */
        for(jidx=j_index_start; jidx<j_index_end; jidx++)
        {
            /* Get j neighbor index, and coordinate index */
            jnr              = jjnr[jidx];
            j_coord_offset   = DIM*jnr;

            /* load j atom coordinates */
            jx0              = x[j_coord_offset+DIM*0+XX];
            jy0              = x[j_coord_offset+DIM*0+YY];
            jz0              = x[j_coord_offset+DIM*0+ZZ];

            /* Calculate displacement vector */
            dx10             = ix1 - jx0;
            dy10             = iy1 - jy0;
            dz10             = iz1 - jz0;
            dx20             = ix2 - jx0;
            dy20             = iy2 - jy0;
            dz20             = iz2 - jz0;
            dx30             = ix3 - jx0;
            dy30             = iy3 - jy0;
            dz30             = iz3 - jz0;

            /* Calculate squared distance and things based on it */
            rsq10            = dx10*dx10+dy10*dy10+dz10*dz10;
            rsq20            = dx20*dx20+dy20*dy20+dz20*dz20;
            rsq30            = dx30*dx30+dy30*dy30+dz30*dz30;

            rinv10           = 1.0/sqrt(rsq10);
            rinv20           = 1.0/sqrt(rsq20);
            rinv30           = 1.0/sqrt(rsq30);

            /* Load parameters for j particles */
            jq0              = charge[jnr+0];

            /**************************
             * CALCULATE INTERACTIONS *
             **************************/

            r10              = rsq10*rinv10;

            qq10             = iq1*jq0;

            /* Calculate table index by multiplying r with table scale and truncate to integer */
            rt               = r10*vftabscale;
            vfitab           = rt;
            vfeps            = rt-vfitab;
            vfitab           = 1*4*vfitab;

            /* CUBIC SPLINE TABLE ELECTROSTATICS */
            F                = vftab[vfitab+1];
            Geps             = vfeps*vftab[vfitab+2];
            Heps2            = vfeps*vfeps*vftab[vfitab+3];
            Fp               = F+Geps+Heps2;
            FF               = Fp+Geps+2.0*Heps2;
            felec            = -qq10*FF*vftabscale*rinv10;

            fscal            = felec;

            /* Calculate temporary vectorial force */
            tx               = fscal*dx10;
            ty               = fscal*dy10;
            tz               = fscal*dz10;

            /* Update vectorial force */
            fix1            += tx;
            fiy1            += ty;
            fiz1            += tz;
            f[j_coord_offset+DIM*0+XX] -= tx;
            f[j_coord_offset+DIM*0+YY] -= ty;
            f[j_coord_offset+DIM*0+ZZ] -= tz;

            /* pairwise forces */
            fda_add_nonbonded_single(fda, inr+1, jnr+0, InteractionType_COULOMB, felec, dx10, dy10, dz10);
            fda_virial_bond(fda, inr+1, jnr+0, fscal, dx10, dy10, dz10);

            /**************************
             * CALCULATE INTERACTIONS *
             **************************/

            r20              = rsq20*rinv20;

            qq20             = iq2*jq0;

            /* Calculate table index by multiplying r with table scale and truncate to integer */
            rt               = r20*vftabscale;
            vfitab           = rt;
            vfeps            = rt-vfitab;
            vfitab           = 1*4*vfitab;

            /* CUBIC SPLINE TABLE ELECTROSTATICS */
            F                = vftab[vfitab+1];
            Geps             = vfeps*vftab[vfitab+2];
            Heps2            = vfeps*vfeps*vftab[vfitab+3];
            Fp               = F+Geps+Heps2;
            FF               = Fp+Geps+2.0*Heps2;
            felec            = -qq20*FF*vftabscale*rinv20;

            fscal            = felec;

            /* Calculate temporary vectorial force */
            tx               = fscal*dx20;
            ty               = fscal*dy20;
            tz               = fscal*dz20;

            /* Update vectorial force */
            fix2            += tx;
            fiy2            += ty;
            fiz2            += tz;
            f[j_coord_offset+DIM*0+XX] -= tx;
            f[j_coord_offset+DIM*0+YY] -= ty;
            f[j_coord_offset+DIM*0+ZZ] -= tz;

            /* pairwise forces */
            fda_add_nonbonded_single(fda, inr+2, jnr+0, InteractionType_COULOMB, felec, dx20, dy20, dz20);
            fda_virial_bond(fda, inr+2, jnr+0, fscal, dx20, dy20, dz20);

            /**************************
             * CALCULATE INTERACTIONS *
             **************************/

            r30              = rsq30*rinv30;

            qq30             = iq3*jq0;

            /* Calculate table index by multiplying r with table scale and truncate to integer */
            rt               = r30*vftabscale;
            vfitab           = rt;
            vfeps            = rt-vfitab;
            vfitab           = 1*4*vfitab;

            /* CUBIC SPLINE TABLE ELECTROSTATICS */
            F                = vftab[vfitab+1];
            Geps             = vfeps*vftab[vfitab+2];
            Heps2            = vfeps*vfeps*vftab[vfitab+3];
            Fp               = F+Geps+Heps2;
            FF               = Fp+Geps+2.0*Heps2;
            felec            = -qq30*FF*vftabscale*rinv30;

            fscal            = felec;

            /* Calculate temporary vectorial force */
            tx               = fscal*dx30;
            ty               = fscal*dy30;
            tz               = fscal*dz30;

            /* Update vectorial force */
            fix3            += tx;
            fiy3            += ty;
            fiz3            += tz;
            f[j_coord_offset+DIM*0+XX] -= tx;
            f[j_coord_offset+DIM*0+YY] -= ty;
            f[j_coord_offset+DIM*0+ZZ] -= tz;

            /* pairwise forces */
            fda_add_nonbonded_single(fda, inr+3, jnr+0, InteractionType_COULOMB, felec, dx30, dy30, dz30);
            fda_virial_bond(fda, inr+3, jnr+0, fscal, dx30, dy30, dz30);

            /* Inner loop uses 114 flops */
        }
        /* End of innermost loop */

        tx = ty = tz = 0;
        f[i_coord_offset+DIM*1+XX] += fix1;
        f[i_coord_offset+DIM*1+YY] += fiy1;
        f[i_coord_offset+DIM*1+ZZ] += fiz1;
        tx                         += fix1;
        ty                         += fiy1;
        tz                         += fiz1;
        f[i_coord_offset+DIM*2+XX] += fix2;
        f[i_coord_offset+DIM*2+YY] += fiy2;
        f[i_coord_offset+DIM*2+ZZ] += fiz2;
        tx                         += fix2;
        ty                         += fiy2;
        tz                         += fiz2;
        f[i_coord_offset+DIM*3+XX] += fix3;
        f[i_coord_offset+DIM*3+YY] += fiy3;
        f[i_coord_offset+DIM*3+ZZ] += fiz3;
        tx                         += fix3;
        ty                         += fiy3;
        tz                         += fiz3;
        fshift[i_shift_offset+XX]  += tx;
        fshift[i_shift_offset+YY]  += ty;
        fshift[i_shift_offset+ZZ]  += tz;

        /* Increment number of inner iterations */
        inneriter                  += j_index_end - j_index_start;

        /* Outer loop uses 30 flops */
    }

    /* Increment number of outer iterations */
    outeriter        += nri;

    /* Update outer/inner flops */

    inc_nrnb(nrnb,eNR_NBKERNEL_ELEC_W4_F,outeriter*30 + inneriter*114);
}
