public class CellModel {
    int lap;
    int posx;
    int posy;
    String status;

    public CellModel(int lap, int posx, int posy, String status) {
        this.lap = lap;
        this.posx = posx;
        this.posy = posy;
        this.status = status;
    }

    public int getLap() {
        return lap;
    }

    public void setLap(int lap) {
        this.lap = lap;
    }

    public int getPosx() {
        return posx;
    }

    public void setPosx(int posx) {
        this.posx = posx;
    }

    public int getPosy() {
        return posy;
    }

    public void setPosy(int posy) {
        this.posy = posy;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }
}
