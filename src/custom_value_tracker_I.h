/* ----------------------------------------------------------------------
   LIGGGHTS - LAMMPS Improved for General Granular and Granular Heat
   Transfer Simulations

   LIGGGHTS is part of the CFDEMproject
   www.liggghts.com | www.cfdem.com

   Christoph Kloss, christoph.kloss@cfdem.com
   Copyright 2009-2012 JKU Linz
   Copyright 2012-     DCS Computing GmbH, Linz

   LIGGGHTS is based on LAMMPS
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   This software is distributed under the GNU General Public License.

   See the README file in the top-level directory.
------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------
   Contributing authors:
   Christoph Kloss (JKU Linz, DCS Computing GmbH, Linz)
   Philippe Seil (JKU Linz)
------------------------------------------------------------------------- */

#ifndef LMP_CUSTOM_VALUE_TRACKER_I_H
#define LMP_CUSTOM_VALUE_TRACKER_I_H

  /* ----------------------------------------------------------------------
   add property
  ------------------------------------------------------------------------- */

  template<typename T>
  T* CustomValueTracker::addElementProperty(char *_id, char* _comm, char* _ref,int _scalePower)
  {
     // error if property exists already
     if(elementProperties_.getPointerById<T>(_id))
     {
         char *errmsg = new char[strlen(_id)+200];
         sprintf(errmsg,"Illegal command, features are incompatible - element property '%s' exists already",_id);
         error->all(FLERR,errmsg);
         delete []errmsg;
     }

     // add property
     elementProperties_.add<T>(_id,_comm,_ref,_scalePower);

     // check if properties were set correctly
     // error here since ContainerBase not derived from Pointers
     if(!elementProperties_.getPointerById<T>(_id)->propertiesSetCorrectly())
     {
         char *errmsg = new char[strlen(_id)+200];
         sprintf(errmsg,"Illegal element property, comm or frame property not set correctly for property '%s'",_id);
         error->all(FLERR,errmsg);
         delete []errmsg;
     }

     // allocate memory and initialize
     elementProperties_.getPointerById<T>(_id)->addUninitialized(owner_.sizeLocal()+owner_.sizeGhost());
     elementProperties_.getPointerById<T>(_id)->setAll(0);

     // return pointer
     return elementProperties_.getPointerById<T>(_id);
  }

  template<typename T>
  T* CustomValueTracker::addMeshProperty(char *_id, char* _comm, char* _ref,int _scalePower)
  {
     // error if property exists already
     if(meshProperties_.getPointerById<T>(_id))
     {
         char *errmsg = new char[strlen(_id)+200];
         sprintf(errmsg,"Illegal command, features are incompatible - mesh property '%s' exists already",_id);
         error->all(FLERR,errmsg);
         delete []errmsg;
     }

     // add property
     meshProperties_.add<T>(_id,_comm,_ref,_scalePower);

     // check if properties were set correctly
     // error here since ContainerBase not derived from Pointers
     if(!meshProperties_.getPointerById<T>(_id)->propertiesSetCorrectly())
     {
         char *errmsg = new char[strlen(_id)+200];
         sprintf(errmsg,"Illegal mesh property, comm or frame property not set correctly for property '%s'",_id);
         error->all(FLERR,errmsg);
         delete []errmsg;
     }

     // allocate memory
     meshProperties_.getPointerById<T>(_id)->addUninitialized(capacityElement_);

     // return pointer
     return meshProperties_.getPointerById<T>(_id);
  }

  /* ----------------------------------------------------------------------
   mem management
  ------------------------------------------------------------------------- */

  void CustomValueTracker::grow(int to)
  {
      elementProperties_.grow(to);
      capacityElement_ = to;
  }

  /* ----------------------------------------------------------------------
   get reference
  ------------------------------------------------------------------------- */

  template<typename T>
  T* CustomValueTracker::getElementProperty(char *_id)
  {
     return elementProperties_.getPointerById<T>(_id);
  }

  template<typename T>
  T* CustomValueTracker::getMeshProperty(char *_id)
  {
     return meshProperties_.getPointerById<T>(_id);
  }

  /* ----------------------------------------------------------------------
   set property
  ------------------------------------------------------------------------- */

  template<typename T, typename U>
  void CustomValueTracker::setElementProperty(char *_id, U def)
  {
     elementProperties_.getPointerById<T>(_id)->setAll(def);
  }
  template<typename T, typename U>
  void CustomValueTracker::setMeshProperty(char *_id, U def)
  {
     meshProperties_.getPointerById<T>(_id)->setAll(def);
  }

  /* ----------------------------------------------------------------------
   delete element i
  ------------------------------------------------------------------------- */

  void CustomValueTracker::deleteElement(int i)
  {
      elementProperties_.deleteElement(i);
  }

  /* ----------------------------------------------------------------------
   delete forward comm properties of element i
  ------------------------------------------------------------------------- */

  void CustomValueTracker::deleteForwardElement(int i,bool scale,bool translate,bool rotate)
  {
      elementProperties_.deleteForwardElement(i,scale,translate,rotate);
  }

  /* ----------------------------------------------------------------------
   move element i
  ------------------------------------------------------------------------- */

  void CustomValueTracker::moveElement(int i, double *delta)
  {
      elementProperties_.moveElement(i,delta);
  }

  /* ----------------------------------------------------------------------
   push / pop for list
  ------------------------------------------------------------------------- */

  int CustomValueTracker::listBufSize(int n,int operation,bool scale,bool translate,bool rotate)
  {
    return elementProperties_.listBufSize(n,operation,scale,translate,rotate);
  }

  int CustomValueTracker::pushListToBuffer(int n, int *list, double *buf, int operation,bool scale,bool translate, bool rotate)
  {
    return elementProperties_.pushListToBuffer(n,list,buf,operation,scale,translate,rotate);
  }

  int CustomValueTracker::popListFromBuffer(int first, int n, double *buf, int operation,bool scale,bool translate, bool rotate)
  {
    return elementProperties_.popListFromBuffer(first,n,buf,operation,scale,translate,rotate);
  }

  /* ----------------------------------------------------------------------
   push / pop for element i
  ------------------------------------------------------------------------- */

  int CustomValueTracker::elemBufSize(int operation,bool scale,bool translate,bool rotate)
  {
    
    return elementProperties_.elemBufSize(operation,scale,translate,rotate);
  }

  int CustomValueTracker::pushElemToBuffer(int i, double *buf, int operation,bool scale,bool translate, bool rotate)
  {
    return elementProperties_.pushElemToBuffer(i,buf,operation,scale,translate,rotate);
  }

  int CustomValueTracker::popElemFromBuffer(double *buf, int operation,bool scale,bool translate, bool rotate)
  {
    return elementProperties_.popElemFromBuffer(buf,operation,scale,translate,rotate);
  }

#endif
