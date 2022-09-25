#include <stdio.h>
#include <stdlib.h> // malloc
 
static double
Determinant(double** matrixA, int uesrN);
 
static int
InverseMatrix(double** matrixA, double** inversedMatrixA, int userN,
                double determinantResult);
 
static int
MultiplicationMatrix(double** matrixA, double** matrixB, double** resultMatrix,
                        int userN);
int
main(void)
{
    int userN = 0;
    int userM = 0;
    int row = 0;
    int column = 0;
 
    double** matrixA;
    double** inversedMatrixA;
    double** multiplicationalMatrixA;
    double determinantResult = 0;
 
    printf("n by n = ");
    scanf("%d", &userN);
 
    userM = userN - 1;
 
    matrixA = (double**)malloc(sizeof(double*) * userN);
    for (row = 0; row < userN; row++)
    {
        matrixA[row] = (double*)malloc(sizeof(double) * userN);
    } /* for (row = 0; row < userN; row++) */
 
    for (row = 0; row < userN; row++)
    {
        for (column = 0; column < userN; column++)
        {
            printf("%d , %d =", row, column);
            scanf("%lf", &matrixA[row][column]);
        } /* for (column = 0; column < userN; column++) */
    } /* for (row = 0; row < userN; row++) */
    printf("\nA =\n");
 
    for (row = 0; row < userN; row++)
    {
        for (column = 0; column < userN; column++)
        {
            printf("%10.5lf ", matrixA[row][column]);
            if(column % userN == userM)
            {
                printf("\n");
            } /* if(column % userN == userM) */
        } /* for (column = 0; column < userN; column++) */
    } /* for (row = 0; row < userN; row++) */
 
    determinantResult = Determinant(matrixA, userN);
    printf("\n|A| = %.5g\n", determinantResult);
 
    if (!determinantResult)
    {
        printf("\nNo determinant(A)\n");
    } /* if (!determinantResult) */
    else
    {
        inversedMatrixA = (double**)malloc(sizeof(double*) * userN);
        multiplicationalMatrixA = (double**)malloc(sizeof(double*) * userN);
 
        for (row = 0; row < userN; row++)
        {
            inversedMatrixA[row] = (double*)malloc(sizeof(double) * userN);
            multiplicationalMatrixA[row] =
                                        (double*)malloc(sizeof(double) * userN);
        } /* for (row = 0; row < userN; row++) */
 
        InverseMatrix(matrixA, inversedMatrixA, userN, determinantResult);
 
        printf("\nA' = \n");
        for (row = 0; row < userN; row++)
        {
            for (column = 0; column < userN; column++)
            {
                printf("%15.5lf ", inversedMatrixA[row][column]);
                if (column % userN == userM)
                {
                    printf("\n");
                } /* if (column % userN == userM) */
            } /* for (column = 0; column < userN; column++) */
        } /* for (row = 0; row < userN; row++) */
 
        MultiplicationMatrix(matrixA, inversedMatrixA,
                                multiplicationalMatrixA, userN);
 
        printf("\nA'A = \n");
        for (row = 0; row < userN; row++)
        {
            for (column = 0; column < userN; column++)
            {
                printf("%15.5lf ", multiplicationalMatrixA[row][column]);
                if (column % userN == userM)
                {
                    printf("\n");
                } /* if (column % userN == userM) */
            } /* for (column = 0; column < userN; column++) */
        } /* for (row = 0; row < userN; row++) */
 
        for(row = 0; row < userN; row++)
        {
            free(multiplicationalMatrixA[row]);
            free(inversedMatrixA[row]);
        } /* for(row = 0; row < userN; row++) */
 
        free(multiplicationalMatrixA);
        free(inversedMatrixA);
    }
 
    for(row = 0; row < userN; row++)
    {
        free(matrixA[row]);
    } /* for(row = 0; row < userN; row++) */
    free(matrixA);
 
    return 0;
}
 
/**
 * Function Name: Determinant
 *
 * Function Description:
 *        This function solve the determinant from a matrix A
 *
 * Input: matrixA , userN
 * Output: determinantResult
 * Version: 1.0
 */
static double
Determinant (double** matrixA, int userN)
{
    int userM = 0;
    int counter = 0;
    int determinantACheck = 0;
    int determinantARow = 0;
    int determinantAColumn = 0;
    int sign = 1;
 
    double determinantResult = 0;
    double** matrixDeterminantA;
 
    userM = userN-1;
 
    matrixDeterminantA = (double**)malloc(sizeof(double*) * userM);
    for (counter = 0; counter < userM; counter++)
    {
        matrixDeterminantA[counter] = (double*)malloc(sizeof(double) * userM);
    } /* for (counter = 0; counter < userM; counter++) */
 
    if (userN == 2)
    {
        determinantResult = (matrixA[0][0] * matrixA[1][1]) -
                                (matrixA[0][1] * matrixA[1][0]);
    } /* if (userN == 2) */
 
    else
    {
        for (counter = 0; counter < userN; counter++)
        {
            for (determinantARow = 0; determinantARow < userM;
                    determinantARow++)
            {
                determinantACheck = 0;
                for (determinantAColumn = 0; determinantAColumn < userM;
                        determinantAColumn++)
                {
                    if (determinantAColumn == counter)
                    {
                        determinantACheck++;
                    } /* if (determinantAColumn == counter) */
                    matrixDeterminantA[determinantARow][determinantAColumn] =
                            matrixA[determinantARow + 1][determinantACheck];
                    determinantACheck++;
                } /* for (determinantAColumn = 0; determinantAColumn < userM;
                        determinantAColumn++) */
            } /* for (determinantARow = 0; determinantARow < userM;
                    determinantARow++) */
 
            if(counter % 2 == 1)
            {
                sign = -1;
            } /* if(counter % 2 == 1) */
            else
            {
                sign = 1;
            } /* else */
 
            determinantResult += sign * matrixA[0][counter] *
                                    Determinant(matrixDeterminantA,userM);
        } /* for (counter = 0; counter < userN; counter++) */
    } /* else */
 
 
    for(counter = 0; counter < userM; counter++)
    {
        free(matrixDeterminantA[counter]);
    } /* for(counter = 0; counter < userM; counter++) */
    free(matrixDeterminantA);
 
    return determinantResult;
}

static int
InverseMatrix(double** matrixA, double** inversedMatrixA, int userN,
                double determinantResult)
{
    int userM = 0;
    int row = 0;
    int column = 0;
    int cofactorRowCheck=0;
    int cofactorColumnCheck=0;
    int cofactorRow=0;
    int cofactorColumn=0;
    int sign = 1;
 
    double temporaryValue = 0;
    double** determinantCofactorMatrixA;
    double** cofactorMatrixA;
 
    userM = userN - 1;
 
    cofactorMatrixA = (double**)malloc(sizeof(double*) * userN);
    for(row = 0; row < userN; row++)
    {
        cofactorMatrixA[row] = (double*)malloc(sizeof(double) * userN);
    } /* for(row = 0; row < userN; row++) */
 
    determinantCofactorMatrixA = (double**)malloc(sizeof(double*) * userM);
    for(row = 0; row < userM; row++)
    {
        determinantCofactorMatrixA[row] =
                (double*)malloc(sizeof(double) * userM);
    } /* for(row = 0; row < userM; row++) */
 
    /* Find the Cofactor */
 
    if (userN == 2)
    {
        cofactorMatrixA[0][0] = (matrixA[0][0] * matrixA[1][1]) -
                                (matrixA[0][1] * matrixA[1][0]);
    } /* if (userN == 2) */
    else
    {
        for (row = 0; row < userN; row++)
        {
            for (column = 0; column < userN; column++)
            {
                cofactorColumnCheck=0;
                for (cofactorRow = 0; cofactorRow < userM; cofactorRow++)
                {
                    if(cofactorColumnCheck == column)
                    {
                        cofactorColumnCheck++;
                    } /* if(cofactorColumnCheck == column) */
                    cofactorRowCheck=0;
                    for (cofactorColumn = 0; cofactorColumn < userM;
                            cofactorColumn++)
                    {
                        if (cofactorColumn == row)
                        {
                            cofactorRowCheck++;
                        } /* if (cofactorColumn == row) */
                        determinantCofactorMatrixA[cofactorRow]
                            [cofactorColumn] =
                            matrixA[cofactorColumnCheck][cofactorRowCheck];
                        cofactorRowCheck++;
                    } /* for (cofactorColumn = 0; cofactorColumn < userM;
                            cofactorColumn++) */
                    cofactorColumnCheck++;
                } /* for (cofactorRow = 0; cofactorRow < userM;
                            cofactorRow++) */
                if( (row+column) % 2 == 1)
                {
                    sign = -1;
                } /* if( (row+column) % 2 == 1) */
                else
                {
                    sign = 1;
                } /* else */
                cofactorMatrixA[row][column] = sign *
                        Determinant(determinantCofactorMatrixA, userM);
            } /* for (column = 0; column < userN; column++) */
        } /* for (row = 0; row < userN; row++) */
    } /* else */
 
 
    for(row = 0; row < userM; row++)
    {
        free(determinantCofactorMatrixA[row]);
    } /* for(row = 0; row < userM; row++) */
    free(determinantCofactorMatrixA);
 
    // adjoint
    for (row = 0; row < userN; row++)
    {
        for (column = 0; column < userN; column++)
        {
            temporaryValue = cofactorMatrixA[row][column];
            inversedMatrixA[row][column] = cofactorMatrixA[column][row];
            inversedMatrixA[column][row] = temporaryValue;
        } /* for (column = 0; column < userN; column++) */
    } /* for (row = 0; row < userN; row++) */
 
    // adjoint / determinant = inverse
    for (row = 0; row < userN;row++)
    {
        for (column = 0; column < userN; column++)
        {
            inversedMatrixA[row][column] =
                            cofactorMatrixA[row][column] / determinantResult;
        } 
    } 
 
    for(row = 0; row < userN; row++)
    {
        free(cofactorMatrixA[row]);
    } 
    free(cofactorMatrixA);
 
    return 0;
}
 
static int
MultiplicationMatrix(double** matrixA, double** matrixB, double** resultMatrix,
                        int userN)
{
    int row = 0;
    int column = 0;
    int check = 0;
 
    for (row = 0; row < userN; row++)
    {
        for (column = 0; column < userN; column++)
        {
            for (check = 0; check < userN; check++)
            {
                resultMatrix[row][column] += matrixA[row][check] *
                                                matrixB[check][column];
            } 
        } 
    } 
    return 0;
}
