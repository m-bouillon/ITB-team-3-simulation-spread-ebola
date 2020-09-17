import java.util.ArrayList;

public class TurnModel {
    ArrayList<CellModel> cells;
    int turn;

    public TurnModel(ArrayList<CellModel> cells, int turn) {
        this.cells = cells;
        this.turn = turn;
    }

    public TurnModel(CellModel cell) {
        this.cells = new ArrayList<>();
        this.cells.add(cell);
        this.turn = cell.lap;
    }


    public ArrayList<CellModel> getCells() {
        return cells;
    }

    public void addCell(CellModel cell){
        this.cells.add(cell);
    }

    public void setCells(ArrayList<CellModel> cells) {
        this.cells = cells;
    }

    public int getTurn() {
        return turn;
    }

    public void setTurn(int turn) {
        this.turn = turn;
    }
}
