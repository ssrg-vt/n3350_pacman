/* ###
 * IP: Apache License 2.0 with LLVM Exceptions
 */
/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.1
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package SWIG;

public final class GdbSignal {
  public final static GdbSignal eGdbSignalBadAccess = new GdbSignal("eGdbSignalBadAccess", lldbJNI.eGdbSignalBadAccess_get());
  public final static GdbSignal eGdbSignalBadInstruction = new GdbSignal("eGdbSignalBadInstruction", lldbJNI.eGdbSignalBadInstruction_get());
  public final static GdbSignal eGdbSignalArithmetic = new GdbSignal("eGdbSignalArithmetic", lldbJNI.eGdbSignalArithmetic_get());
  public final static GdbSignal eGdbSignalEmulation = new GdbSignal("eGdbSignalEmulation", lldbJNI.eGdbSignalEmulation_get());
  public final static GdbSignal eGdbSignalSoftware = new GdbSignal("eGdbSignalSoftware", lldbJNI.eGdbSignalSoftware_get());
  public final static GdbSignal eGdbSignalBreakpoint = new GdbSignal("eGdbSignalBreakpoint", lldbJNI.eGdbSignalBreakpoint_get());

  public final int swigValue() {
    return swigValue;
  }

  public String toString() {
    return swigName;
  }

  public static GdbSignal swigToEnum(int swigValue) {
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (int i = 0; i < swigValues.length; i++)
      if (swigValues[i].swigValue == swigValue)
        return swigValues[i];
    throw new IllegalArgumentException("No enum " + GdbSignal.class + " with value " + swigValue);
  }

  private GdbSignal(String swigName) {
    this.swigName = swigName;
    this.swigValue = swigNext++;
  }

  private GdbSignal(String swigName, int swigValue) {
    this.swigName = swigName;
    this.swigValue = swigValue;
    swigNext = swigValue+1;
  }

  private GdbSignal(String swigName, GdbSignal swigEnum) {
    this.swigName = swigName;
    this.swigValue = swigEnum.swigValue;
    swigNext = this.swigValue+1;
  }

  private static GdbSignal[] swigValues = { eGdbSignalBadAccess, eGdbSignalBadInstruction, eGdbSignalArithmetic, eGdbSignalEmulation, eGdbSignalSoftware, eGdbSignalBreakpoint };
  private static int swigNext = 0;
  private final int swigValue;
  private final String swigName;
}
