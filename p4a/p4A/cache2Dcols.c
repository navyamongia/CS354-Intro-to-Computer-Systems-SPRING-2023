//Navya Mongia 
#define num_cols 500
#define num_rows 3000
int arr2D[num_rows][num_cols];

int main() 
{
    for(int col = 0; col < num_cols; col++)
    {
        for(int row = 0; row < num_rows; row++)
        {
            arr2D[row][col] = row + col;
        }
    }

    return 0;
}
