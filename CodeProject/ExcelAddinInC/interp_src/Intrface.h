#define		kFunctionCount	2
#define		kMaxFuncParms	13

const char	*gModuleName = "\024Interpolation Add-In";
const int	gFunctionCount = kFunctionCount,
			gMaxFuncParms = kMaxFuncParms;

LPSTR functionParms[kFunctionCount][kMaxFuncParms] =
{
//	function title, argument types, function name, arg names, type (1=func,2=cmd),
//		group name (func wizard), hotkey, help ID, func help string, (repeat) argument help strings

    {" Interp",	" RRRRA",	" Interp",	" x,xArray,yArray,extrapFlag",	" 1",
    	" Interpolation Add-In",	" ",
    	" interp.hlp!300",
    	" Performs linear interpolation.  This is the general version that can handle single values or arrays.",
    	" The X values to be interpolated.  Can be a single value or an array (each value is interpolated individually)",
    	" A table of values that define X.  Must be sorted (increasing or decreasing).",
    	" A table of values that define Y (for each X table value).",
    	" If TRUE, extrapolation beyond the table is allowed. If omitted or FALSE, the result is truncated at the table bounds." },

    {" InterpX",	" RBRRA",	" InterpX",	" x,xArray,yArray,extrapFlag",	" 1",
    	" Interpolation Add-In",	" ",
    	" interp.hlp!310",
    	" Performs linear interpolation.  This version interpolates only a single X value at a time.",
    	" The X value to be interpolated.  Only a single value is allowed to take advantage of Excel's 'implicit intersection'.",
    	" A table of values that define X.  Must be sorted (increasing or decreasing).",
    	" A table of values that define Y (for each X table value).",
    	" If TRUE, extrapolation beyond the table is allowed. if omitted or FALSE, the result is truncated at the table bounds." },
  };

LPSTR *gFuncParms = &functionParms[0][0];
