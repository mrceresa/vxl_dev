C
C***********************************************************************
C
      SUBROUTINE SLARAN(N, X)
C
C  THIS SUBROUTINE SETS THE VECTOR X TO RANDOM NUMBERS
C
C  FORMAL PARAMETERS
C
      INTEGER N
      REAL X(N)
C
C  LOCAL VARIABLES
C
      INTEGER I, IURAND
C
C  FUNCTIONS CALLED
C
      REAL URAND
C
C  SUBROUTINES CALLED
C
C     NONE
C
C  INITIALIZE SEED
C
      DATA IURAND /0/
C
      DO 10 I = 1, N
         X(I) = URAND(IURAND) - 0.5 
   10 CONTINUE
      RETURN
      END
