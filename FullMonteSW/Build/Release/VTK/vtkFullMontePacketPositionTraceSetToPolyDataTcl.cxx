// tcl wrapper for vtkFullMontePacketPositionTraceSetToPolyData object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkFullMontePacketPositionTraceSetToPolyData.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkFullMontePacketPositionTraceSetToPolyDataNewCommand()
{
  vtkFullMontePacketPositionTraceSetToPolyData *temp = vtkFullMontePacketPositionTraceSetToPolyData::New();
  return static_cast<ClientData>(temp);
}

int vtkObjectCppCommand(vtkObject *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkFullMontePacketPositionTraceSetToPolyDataCppCommand(vtkFullMontePacketPositionTraceSetToPolyData *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int vtkFullMontePacketPositionTraceSetToPolyDataCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkFullMontePacketPositionTraceSetToPolyDataCppCommand(static_cast<vtkFullMontePacketPositionTraceSetToPolyData *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkFullMontePacketPositionTraceSetToPolyData_TclCreate(Tcl_Interp *interp)
{
  vtkTclCreateNew(interp,const_cast<char *>("vtkFullMontePacketPositionTraceSetToPolyData"),vtkFullMontePacketPositionTraceSetToPolyDataNewCommand,vtkFullMontePacketPositionTraceSetToPolyDataCommand);
  return 0;
}

int VTKTCL_EXPORT vtkFullMontePacketPositionTraceSetToPolyDataCppCommand(vtkFullMontePacketPositionTraceSetToPolyData *op, Tcl_Interp *interp,
             int argc, char *argv[])
{
  int    tempi = 0;      (void)tempi;
  double tempd = 0.0;    (void)tempd;
  static char temps[80]; (void)temps;
  int    error = 0;      (void)error;
  temps[0] = 0;

  if (argc < 2)
    {
    Tcl_SetResult(interp,const_cast<char *>("Could not find requested method."), TCL_VOLATILE);
    return TCL_ERROR;
    }
  if (!interp)
    {
    if (!strcmp("DoTypecasting",argv[0]))
      {
      if (!strcmp("vtkFullMontePacketPositionTraceSetToPolyData",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkObjectCppCommand(static_cast<vtkObject *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkObject"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("GetClassName",argv[1]))&&(argc == 2))
    {
    const char    *temp20;
    temp20 = (op)->GetClassName();
    if (temp20)
      {
      Tcl_SetResult(interp, const_cast<char *>(temp20), TCL_VOLATILE);
      }
    else
      {
      Tcl_ResetResult(interp);
      }
    return TCL_OK;
    }
  if ((!strcmp("IsA",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->IsA(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("NewInstance",argv[1]))&&(argc == 2))
    {
    vtkFullMontePacketPositionTraceSetToPolyData  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkFullMontePacketPositionTraceSetToPolyData");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkFullMontePacketPositionTraceSetToPolyData  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkFullMontePacketPositionTraceSetToPolyData");
    return TCL_OK;
    }
    }
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkFullMontePacketPositionTraceSetToPolyData  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkFullMontePacketPositionTraceSetToPolyData");
    return TCL_OK;
    }
  if ((!strcmp("source",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->source(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("update",argv[1]))&&(argc == 2))
    {
    op->update();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("getPolyData",argv[1]))&&(argc == 2))
    {
    vtkPolyData  *temp20;
    temp20 = (op)->getPolyData();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPolyData");
    return TCL_OK;
    }
  if ((!strcmp("includeWeight",argv[1]))&&(argc == 3))
    {
    bool   temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi ? true : false;
    if (!error)
    {
    op->includeWeight(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("includeLogWeight",argv[1]))&&(argc == 3))
    {
    bool   temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi ? true : false;
    if (!error)
    {
    op->includeLogWeight(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("includeTime",argv[1]))&&(argc == 3))
    {
    bool   temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi ? true : false;
    if (!error)
    {
    op->includeTime(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("includeLength",argv[1]))&&(argc == 3))
    {
    bool   temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi ? true : false;
    if (!error)
    {
    op->includeLength(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("includeSteps",argv[1]))&&(argc == 3))
    {
    bool   temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi ? true : false;
    if (!error)
    {
    op->includeSteps(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("includeTetra",argv[1]))&&(argc == 3))
    {
    bool   temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi ? true : false;
    if (!error)
    {
    op->includeTetra(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("includeMaterial",argv[1]))&&(argc == 3))
    {
    bool   temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi ? true : false;
    if (!error)
    {
    op->includeMaterial(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkFullMontePacketPositionTraceSetToPolyDataCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkObjectCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkFullMontePacketPositionTraceSetToPolyData:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  source\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  update\n",NULL);
    Tcl_AppendResult(interp,"  getPolyData\n",NULL);
    Tcl_AppendResult(interp,"  includeWeight\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  includeLogWeight\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  includeTime\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  includeLength\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  includeSteps\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  includeTetra\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  includeMaterial\t with 1 arg\n",NULL);
    return TCL_OK;
    }

  if (!strcmp("DescribeMethods",argv[1]))
    {
    if(argc>3) {
      Tcl_SetResult ( interp, const_cast<char*>("Wrong number of arguments: object DescribeMethods <MethodName>"), TCL_VOLATILE ); 
      return TCL_ERROR;
 }
    if(argc==2) {

  Tcl_DString dString, dStringParent;

  Tcl_DStringInit ( &dString );

  Tcl_DStringInit ( &dStringParent );
    vtkObjectCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "source" );
    Tcl_DStringAppendElement ( &dString, "update" );
    Tcl_DStringAppendElement ( &dString, "getPolyData" );
    Tcl_DStringAppendElement ( &dString, "includeWeight" );
    Tcl_DStringAppendElement ( &dString, "includeLogWeight" );
    Tcl_DStringAppendElement ( &dString, "includeTime" );
    Tcl_DStringAppendElement ( &dString, "includeLength" );
    Tcl_DStringAppendElement ( &dString, "includeSteps" );
    Tcl_DStringAppendElement ( &dString, "includeTetra" );
    Tcl_DStringAppendElement ( &dString, "includeMaterial" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkObjectCppCommand(op,interp,argc,argv);
    if ( SuperClassStatus == TCL_OK ) { return TCL_OK; }
    /* Starting function: GetClassName */
    if ( strcmp ( argv[2], "GetClassName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetClassName */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "const char *GetClassName();" );
    Tcl_DStringAppendElement ( &dString, "vtkFullMontePacketPositionTraceSetToPolyData" );
    /* Closing for GetClassName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: IsA */
    if ( strcmp ( argv[2], "IsA" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for IsA */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int IsA(const char *name);" );
    Tcl_DStringAppendElement ( &dString, "vtkFullMontePacketPositionTraceSetToPolyData" );
    /* Closing for IsA */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: NewInstance */
    if ( strcmp ( argv[2], "NewInstance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for NewInstance */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkFullMontePacketPositionTraceSetToPolyData *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkFullMontePacketPositionTraceSetToPolyData" );
    /* Closing for NewInstance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SafeDownCast */
    if ( strcmp ( argv[2], "SafeDownCast" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkObject" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SafeDownCast */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkFullMontePacketPositionTraceSetToPolyData *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkFullMontePacketPositionTraceSetToPolyData" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: New */
    if ( strcmp ( argv[2], "New" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "New" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for New */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkFullMontePacketPositionTraceSetToPolyData *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkFullMontePacketPositionTraceSetToPolyData" );
    /* Closing for New */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: source */
    if ( strcmp ( argv[2], "source" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "source" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for source */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void source(const char *swigPtrString);" );
    Tcl_DStringAppendElement ( &dString, "vtkFullMontePacketPositionTraceSetToPolyData" );
    /* Closing for source */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: update */
    if ( strcmp ( argv[2], "update" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "update" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for update */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void update();" );
    Tcl_DStringAppendElement ( &dString, "vtkFullMontePacketPositionTraceSetToPolyData" );
    /* Closing for update */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: getPolyData */
    if ( strcmp ( argv[2], "getPolyData" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "getPolyData" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for getPolyData */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData *getPolyData() const;" );
    Tcl_DStringAppendElement ( &dString, "vtkFullMontePacketPositionTraceSetToPolyData" );
    /* Closing for getPolyData */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: includeWeight */
    if ( strcmp ( argv[2], "includeWeight" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "includeWeight" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "bool" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for includeWeight */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void includeWeight(bool e);" );
    Tcl_DStringAppendElement ( &dString, "vtkFullMontePacketPositionTraceSetToPolyData" );
    /* Closing for includeWeight */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: includeLogWeight */
    if ( strcmp ( argv[2], "includeLogWeight" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "includeLogWeight" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "bool" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for includeLogWeight */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void includeLogWeight(bool e);" );
    Tcl_DStringAppendElement ( &dString, "vtkFullMontePacketPositionTraceSetToPolyData" );
    /* Closing for includeLogWeight */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: includeTime */
    if ( strcmp ( argv[2], "includeTime" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "includeTime" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "bool" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for includeTime */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void includeTime(bool e);" );
    Tcl_DStringAppendElement ( &dString, "vtkFullMontePacketPositionTraceSetToPolyData" );
    /* Closing for includeTime */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: includeLength */
    if ( strcmp ( argv[2], "includeLength" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "includeLength" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "bool" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for includeLength */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void includeLength(bool e);" );
    Tcl_DStringAppendElement ( &dString, "vtkFullMontePacketPositionTraceSetToPolyData" );
    /* Closing for includeLength */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: includeSteps */
    if ( strcmp ( argv[2], "includeSteps" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "includeSteps" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "bool" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for includeSteps */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void includeSteps(bool e);" );
    Tcl_DStringAppendElement ( &dString, "vtkFullMontePacketPositionTraceSetToPolyData" );
    /* Closing for includeSteps */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: includeTetra */
    if ( strcmp ( argv[2], "includeTetra" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "includeTetra" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "bool" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for includeTetra */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void includeTetra(bool e);" );
    Tcl_DStringAppendElement ( &dString, "vtkFullMontePacketPositionTraceSetToPolyData" );
    /* Closing for includeTetra */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: includeMaterial */
    if ( strcmp ( argv[2], "includeMaterial" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "includeMaterial" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "bool" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for includeMaterial */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void includeMaterial(bool e);" );
    Tcl_DStringAppendElement ( &dString, "vtkFullMontePacketPositionTraceSetToPolyData" );
    /* Closing for includeMaterial */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkObjectCppCommand(static_cast<vtkObject *>(op),interp,argc,argv) == TCL_OK)
    {
    return TCL_OK;
    }
    }
  catch (std::exception &e)
    {
    Tcl_AppendResult(interp, "Uncaught exception: ",  e.what(), "\n", NULL);
    return TCL_ERROR;
    }
  return TCL_ERROR;
}
