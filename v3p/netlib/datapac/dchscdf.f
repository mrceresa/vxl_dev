      SUBROUTINE DCHSCDF(X,NU,CDF)
C
C     PURPOSE--THIS SUBROUTINE COMPUTES THE CUMULATIVE DISTRIBUTION
C              FUNCTION VALUE FOR THE CHI-SQUARED DISTRIBUTION
C              WITH INTEGER DEGREES OF FREEDOM PARAMETER = NU.
C              THIS DISTRIBUTION IS DEFINED FOR ALL NON-NEGATIVE X.
C              THE PROBABILITY DENSITY FUNCTION IS GIVEN
C              IN THE REFERENCES BELOW. 
C     INPUT  ARGUMENTS--X      = THE SINGLE PRECISION VALUE AT
C                                WHICH THE CUMULATIVE DISTRIBUTION
C                                FUNCTION IS TO BE EVALUATED.
C                                X SHOULD BE NON-NEGATIVE.
C                     --NU     = THE INTEGER NUMBER OF DEGREES
C                                OF FREEDOM.
C                                NU SHOULD BE POSITIVE.
C     OUTPUT ARGUMENTS--CDF    = THE SINGLE PRECISION CUMULATIVE
C                                DISTRIBUTION FUNCTION VALUE.
C     OUTPUT--THE SINGLE PRECISION CUMULATIVE DISTRIBUTION
C             FUNCTION VALUE CDF FOR THE CHI-SQUARED DISTRIBUTION
C             WITH DEGREES OF FREEDOM PARAMETER = NU.
C     PRINTING--NONE UNLESS AN INPUT ARGUMENT ERROR CONDITION EXISTS. 
C     RESTRICTIONS--X SHOULD BE NON-NEGATIVE.
C                 --NU SHOULD BE A POSITIVE INTEGER VARIABLE.
C     OTHER DATAPAC   SUBROUTINES NEEDED--NORCDF. 
C     FORTRAN LIBRARY SUBROUTINES NEEDED--DSQRT, DEXP.
C     MODE OF INTERNAL OPERATIONS--DOUBLE PRECISION.
C     LANGUAGE--ANSI FORTRAN. 
C     REFERENCES--NATIONAL BUREAU OF STANDARDS APPLIED MATHEMATICS
C                 SERIES 55, 1964, PAGE 941, FORMULAE 26.4.4 AND 26.4.5.
C               --JOHNSON AND KOTZ, CONTINUOUS UNIVARIATE
C                 DISTRIBUTIONS--1, 1970, PAGE 176,
C                 FORMULA 28, AND PAGE 180, FORMULA 33.1.
C               --OWEN, HANDBOOK OF STATISTICAL TABLES,
C                 1962, PAGES 50-55.
C               --PEARSON AND HARTLEY, BIOMETRIKA TABLES
C                 FOR STATISTICIANS, VOLUME 1, 1954,
C                 PAGES 122-131.
C     WRITTEN BY--JAMES J. FILLIBEN
C                 STATISTICAL ENGINEERING LABORATORY (205.03)
C                 NATIONAL BUREAU OF STANDARDS
C                 WASHINGTON, D. C. 20234
C                 PHONE:  301-921-2315
C     ORIGINAL VERSION--JUNE      1972. 
C     UPDATED         --MAY       1974. 
C     UPDATED         --SEPTEMBER 1975. 
C     UPDATED         --NOVEMBER  1975. 
C     UPDATED         --OCTOBER   1976. 
C
C---------------------------------------------------------------------
C
      DOUBLE PRECISION X
      INTEGER NU
      DOUBLE PRECISION CDF
      DOUBLE PRECISION DX,PI,CHI,SUM,TERM,AI,DCDFN
      DOUBLE PRECISION DNU
      DOUBLE PRECISION DSQRT,DEXP
      DOUBLE PRECISION DLOG
      DOUBLE PRECISION DFACT,DPOWER
      DOUBLE PRECISION DW
      DOUBLE PRECISION D1,D2,D3
      DOUBLE PRECISION TERM0,TERM1,TERM2,TERM3,TERM4
      DOUBLE PRECISION B11
      DOUBLE PRECISION B21
      DOUBLE PRECISION B31,B32
      DOUBLE PRECISION B41,B42,B43
      DATA NUCUT/1000/
      DATA PI/3.14159265358979D0/
      DATA DPOWER/0.33333333333333D0/
      DATA B11/0.33333333333333D0/
      DATA B21/-0.02777777777778D0/
      DATA B31/-0.00061728395061D0/
      DATA B32/-13.0D0/
      DATA B41/0.00018004115226D0/
      DATA B42/6.0D0/
      DATA B43/17.0D0/
C
      IPR=6
C
C     CHECK THE INPUT ARGUMENTS FOR ERRORS
C
      IF(NU.LE.0)GOTO50
      IF(X.LT.0.0)GOTO55
      GOTO90
   50 WRITE(IPR,15) 
      WRITE(IPR,47)NU
      CDF=0.0
      RETURN
   55 WRITE(IPR,4)
      WRITE(IPR,46)X
      CDF=0.0
      RETURN
   90 CONTINUE
    4 FORMAT(1H , 96H***** NON-FATAL DIAGNOSTIC--THE FIRST  INPUT ARGUME
     1NT TO THE DCHSCDF SUBROUTINE IS NEGATIVE *****)
   15 FORMAT(1H , 91H***** FATAL ERROR--THE SECOND INPUT ARGUMENT TO THE
     1 DCHSCDF SUBROUTINE IS NON-POSITIVE *****)
   46 FORMAT(1H , 35H***** THE VALUE OF THE ARGUMENT IS ,E15.8,6H *****)
   47 FORMAT(1H , 35H***** THE VALUE OF THE ARGUMENT IS ,I8   ,6H *****)
C
C-----START POINT-----------------------------------------------------
C
      DX=X
      ANU=NU
      DNU=NU
C
C     IF X IS NON-POSITIVE, SET CDF = 0.0 AND RETURN.
C     IF NU IS SMALLER THAN 10 AND X IS MORE THAN 200
C     STANDARD DEVIATIONS BELOW THE MEAN,
C     SET CDF = 0.0 AND RETURN.
C     IF NU IS 10 OR LARGER AND X IS MORE THAN 100
C     STANDARD DEVIATIONS BELOW THE MEAN,
C     SET CDF = 0.0 AND RETURN.
C     IF NU IS SMALLER THAN 10 AND X IS MORE THAN 200
C     STANDARD DEVIATIONS ABOVE THE MEAN,
C     SET CDF = 1.0 AND RETURN.
C     IF NU IS 10 OR LARGER AND X IS MORE THAN 100
C     STANDARD DEVIATIONS ABOVE THE MEAN,
C     SET CDF = 1.0 AND RETURN.
C
      IF(X.LE.0.0)GOTO105
      AMEAN=ANU
      SD=SQRT(2.0*ANU)
      Z=(X-AMEAN)/SD
      IF(NU.LT.10.AND.Z.LT.-200.0)GOTO105
      IF(NU.GE.10.AND.Z.LT.-100.0)GOTO105
      IF(NU.LT.10.AND.Z.GT.200.0)GOTO107
      IF(NU.GE.10.AND.Z.GT.100.0)GOTO107
      GOTO109
  105 CDF=0.0
      RETURN
  107 CDF=1.0
      RETURN
  109 CONTINUE
C
C     DISTINGUISH BETWEEN 3 SEPARATE REGIONS
C     OF THE (X,NU) SPACE.
C     BRANCH TO THE PROPER COMPUTATIONAL METHOD
C     DEPENDING ON THE REGION.
C     NUCUT HAS THE VALUE 1000.
C
      IF(NU.LT.NUCUT)GOTO1000 
      IF(NU.GE.NUCUT.AND.X.LE.ANU)GOTO2000
      IF(NU.GE.NUCUT.AND.X.GT.ANU)GOTO3000
      IBRAN=1
      WRITE(IPR,99)IBRAN
   99 FORMAT(1H ,42H*****INTERNAL ERROR IN DCHSCDF SUBROUTINE--,
     146HIMPOSSIBLE BRANCH CONDITION AT BRANCH POINT = ,I8) 
      RETURN
C
C     TREAT THE SMALL AND MODERATE DEGREES OF FREEDOM CASE
C     (THAT IS, WHEN NU IS SMALLER THAN 1000).
C     METHOD UTILIZED--EXACT FINITE SUM 
C     (SEE AMS 55, PAGE 941, FORMULAE 26.4.4 AND 26.4.5).
C
 1000 CONTINUE
      CHI=DSQRT(DX) 
      IEVODD=NU-2*(NU/2)
      IF(IEVODD.EQ.0)GOTO120
C
      SUM=0.0D0
      TERM=1.0/CHI
      IMIN=1
      IMAX=NU-1
      GOTO130
C
  120 SUM=1.0D0
      TERM=1.0D0
      IMIN=2
      IMAX=NU-2
C
  130 IF(IMIN.GT.IMAX)GOTO160 
      DO100I=IMIN,IMAX,2
      AI=I
      TERM=TERM*(DX/AI)
      SUM=SUM+TERM
  100 CONTINUE
  160 CONTINUE
C
      SUM=SUM*DEXP(-DX/2.0D0) 
      IF(IEVODD.EQ.0)GOTO170
      SUM=(DSQRT(2.0D0/PI))*SUM
      SPCHI=CHI
      CALL NORCDF(SPCHI,CDFN) 
      DCDFN=CDFN
      SUM=SUM+2.0D0*(1.0D0-DCDFN)
  170 CDF=1.0D0-SUM 
      RETURN
C
C     TREAT THE CASE WHEN NU IS LARGE
C     (THAT IS, WHEN NU IS EQUAL TO OR GREATER THAN 1000)
C     AND X IS LESS THAN OR EQUAL TO NU.
C     METHOD UTILIZED--WILSON-HILFERTY APPROXIMATION
C     (SEE JOHNSON AND KOTZ, VOLUME 1, PAGE 176, FORMULA 28).
C
 2000 CONTINUE
      DFACT=4.5D0*DNU
      U=(((DX/DNU)**DPOWER)-1.0D0+(1.0D0/DFACT))*DSQRT(DFACT)
      CALL NORCDF(U,CDFN)
      CDF=CDFN
      RETURN
C
C     TREAT THE CASE WHEN NU IS LARGE
C     (THAT IS, WHEN NU IS EQUAL TO OR GREATER THAN 1000)
C     AND X IS LARGER THAN NU.
C     METHOD UTILIZED--HILL'S ASYMPTOTIC EXPANSION
C     (SEE JOHNSON AND KOTZ, VOLUME 1, PAGE 180, FORMULA 33.1).
C
 3000 CONTINUE
      DW=DSQRT(DX-DNU-DNU*DLOG(DX/DNU)) 
      DANU=DSQRT(2.0D0/DNU)
      D1=DW
      D2=DW**2
      D3=DW**3
      TERM0=DW
      TERM1=B11*DANU
      TERM2=B21*D1*(DANU**2)
      TERM3=B31*(D2+B32)*(DANU**3)
      TERM4=B41*(B42*D3+B43*D1)*(DANU**4)
      U=TERM0+TERM1+TERM2+TERM3+TERM4
      CALL NORCDF(U,CDFN)
      CDF=CDFN
      RETURN
C
      END 
