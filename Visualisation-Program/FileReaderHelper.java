import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

/**
 * @brief class to read output file from ebola simulation
 */
public class FileReaderHelper {

    public static ArrayList<CellModel> readFileAndFillArray(){

        ArrayList<CellModel> cells = new ArrayList<>();
        try{
            //URL url = getClass().getResource("LetterCountAscending.txt");
            String uri = "../Parallel-Program/out/history.txt";
            BufferedReader br = new BufferedReader(new FileReader(uri));
            try {
                String line = br.readLine();
                while (line != null) {
                    //Split letter and frequency
                    String[] separateS = line.split(" ");
                    CellModel tmpCell = new CellModel(Integer.parseInt(separateS[0]),Integer.parseInt(separateS[1]), Integer.parseInt(separateS[2]),separateS[3]);
                    cells.add(tmpCell);
                    line = br.readLine();
                }

            } finally {
                br.close();
            }
        }catch (IOException e){
            e.printStackTrace();
        }
        return cells;
    }
}
