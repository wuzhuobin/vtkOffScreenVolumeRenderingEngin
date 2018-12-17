/**
 * @file		vtkVolumeViewer.h
 * @author	WUZHUOBIN jiejin2022@163.com
 * @since   Dec.17.2018
 * @date    Dec.17.2018
 * @copyright
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	This program is distributed in the hope that it will be useful, but	<br>
	WITHOUT ANY WARRANTY; without even the implied warranty of <br>
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. <br>
	See the LICENSE for more detail. <br> 
	Copyright (c) WUZHUOBIN. All rights reserved. <br>
	See COPYRIGHT for more detail. <br>
	This software is distributed WITHOUT ANY WARRANTY; without even <br>
	the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR <br>
	PURPOSE.  See the above copyright notice for more information. <br>
	Internal usage only, without the permission of the author, please DO <br>
	NOT publish and distribute without the author's permission. <br>
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
#ifndef __VTK_VOLUME_VIEWER_H__
#define __VTK_VOLUME_VIEWER_H__
#pragma once
// me
#include "vtkoffscreenvolumerenderingengin_export.h"
// vtk
#include <vtkObject.h>
class vtkAlgorithm;
class vtkAlgorithmOutput;
class vtkVolume;
class vtkImageData;
class vtkInformation;
class vtkInteractorObserver;
class vtkRenderWindow;
class vtkRenderer;
class vtkRenderWindowInteractor;
/**
 * @class vtkVolumeViewer
 * @brief  Display a 3D image in volume. 
 * @author WUZHUOBIN
 * @date   Dec.17.2018
 * @see vtkImageViewer2
 * 
 * vtkVolumeViewer is a convenience class for displaying a 3D volume image.
 * It works just like vtkImageViewer, which is exactly the same pipeline. The 
 * class internally supports a few volume rendering presets, #EnumPreset. 
 */
class VTKOFFSCREENVOLUMERENDERINGENGIN_EXPORT vtkVolumeViewer: public vtkObject
{
public: 
  static vtkVolumeViewer *New();
  vtkTypeMacro(vtkVolumeViewer, vtkObject);
  virtual void PrintSelf(ostream &os, vtkIndent indent) VTK_OVERRIDE;

   /**
   * Get the name of rendering window.
   */
  virtual const char *GetWindowName();

  /**
   * Render the resulting image.
   */
  virtual void Render(void);
  virtual void ResetCamera();
  virtual void ResetCameraClippingRange();

  //@{
  /**
   * Set/Get the input image to the viewer.
   */
  virtual void SetInputData(vtkImageData *in);
  virtual vtkImageData *GetInput();
  virtual void SetInputConnection(vtkAlgorithmOutput* input);
  //@}
  typedef enum
  {
      NONE = 0,
      CT_AAA,
      CT_AAA2,
      CT_BONE,
      CT_BONES,
      CT_CARDIAC,
      CT_CARDIAC2,
      CT_CARDIAC3,
      CT_CHEST_CONTRAST_ENHANCED,
      CT_CHEST_VESSELS,
      CT_CORONARY_ARTERIES,
      CT_CORONARY_ARTERIES2,
      CT_CORONARY_ARTERIES3,
      CT_CROPPED_VOLUME_BONE,
      CT_FAT,
      CT_LIVER_VASCULATURE,
      CT_LUNG,
      CT_MIP,
      CT_MUSCLE,
      CT_PULMONARY_ARTERIES,
      CT_SOFT_TISSUE,
      MR_ANGIO,
      MR_DEFAULT,
      MR_MIP,
      MRT2_BRAIN,
      CBCT_DENTAL,
      CBCT_DENTAL_PHANTOM
  } EnumPreset;
  /**
   * @brief Set the Preset.
   * @param prest Set the #Preset.
   * 
   * The Preset is clamped 0~26, which is related to #EnumPreset. 
   */
  virtual void SetPreset(int prest);
  /**
   * @brief Get the Preset
   * @return  Preset
   * 
   */
  vtkGetMacro(Preset, int);
  
  /**
   * @brief Set the Shift.
   * @param shift 
   * 
   * Set the Shift of the scale of image data. 
   */
  virtual void SetShift(double shift);
  /**
   * @brief  Get the Shift. 
   * @return Shift
   */
  vtkGetMacro(Shift, double);

  /**
   * @brief Set the Opacity.
   * @param opacity 
   * 
   * Set the opacity of the volume rendering.
   */
  virtual void SetOpacity(double opacity);
  /**
   * @brief Get the Opacity. 
   * @return Opacity
   */
  vtkGetMacro(Opacity, double);
  // //@{
  // /**
  //  * These are here when using a Tk window.
  //  */
  virtual void SetDisplayId(void *a);
  virtual void SetWindowId(void *a);
  virtual void SetParentId(void *a);
  // //@}

  //@{
  /**
   * Set/Get the position in screen coordinates of the rendering window.
   */
  virtual int* GetPosition();
  virtual void SetPosition(int a,int b);
  virtual void SetPosition(int a[2]) { this->SetPosition(a[0],a[1]); }
  //@}

  //@{
  /**
   * Set/Get the size of the window in screen coordinates in pixels.
   */
  virtual int* GetSize();
  virtual void SetSize(int a, int b);
  virtual void SetSize(int a[2]) { this->SetSize(a[0],a[1]); }
  //@}

  //@{
  /**
   * Get the internal render window, renderer, image actor, and
   * image map instances.
   */
  vtkGetObjectMacro(Volume, vtkVolume);
  //@}

  //@{
  /**
   * Set your own renderwindow and renderer
   */
  vtkGetObjectMacro(RenderWindow,vtkRenderWindow);
  virtual void SetRenderWindow(vtkRenderWindow *arg);
  vtkGetObjectMacro(Renderer, vtkRenderer);
  virtual void SetRenderer(vtkRenderer *arg);
  //@}

  /**
   * Attach an interactor for the internal render window.
   */
  vtkGetObjectMacro(Interactor, vtkRenderWindowInteractor);
  virtual void SetupInteractor(vtkRenderWindowInteractor*);
  virtual vtkInteractorObserver* GetInteractorStyle();

  //@{
  /**
   * Create a window in memory instead of on the screen. This may not
   * be supported for every type of window and on some windows you may
   * need to invoke this prior to the first render.
   */
  virtual void SetOffScreenRendering(int off);
  virtual bool GetOffScreenRendering();
  vtkBooleanMacro(OffScreenRendering, bool);
  //@}

protected:
  vtkVolumeViewer();
  virtual ~vtkVolumeViewer() VTK_OVERRIDE;

  virtual void InstallPipeline();
  virtual void UnInstallPipeline();

  vtkRenderWindowInteractor       *Interactor;
  vtkRenderWindow                 *RenderWindow;
  vtkRenderer                     *Renderer;
  vtkVolume                       *Volume;

  bool FirstRender;
  int Preset;           ///< Represent the current volume rendering preset. 
  double Shift;         ///< Shift the scale before volume rendering. 
  double Opacity;       ///< The opacity of volume rendering.

  vtkAlgorithm* GetInputAlgorithm();
  vtkInformation* GetInputInformation();

private:
  vtkVolumeViewer(const vtkVolumeViewer&) VTK_DELETE_FUNCTION;
  void operator=(const vtkVolumeViewer&) VTK_DELETE_FUNCTION;
};
#endif //!__VTK_VOLUME_VIEWER_H__