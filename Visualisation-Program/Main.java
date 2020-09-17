import javafx.application.Application;
import javafx.beans.value.ObservableValue;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.control.TextField;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

import java.util.ArrayList;

public class Main extends Application {

    HBox hBox;
    VBox vBox;
    Label lbSuceptible, lIncubation, lInfectious, lIsolated, lImmunity, lDead, turnNumber;
    TextField tvTurn;
    Slider slider;

    Canvas boardCanvas;
    int x,y;

    int maxCellY = 0;
    int maxCellX = 0;

    ArrayList<CellModel> cells;
    ArrayList<TurnModel> turns;




    public static void main(String[] args){
        launch(args);
    }

    private static int getTurn(ArrayList<TurnModel> turns, int turnNb){
        for(int i = 0 ; i<turns.size();i++){
            if(turns.get(i).getTurn() == turnNb)
                return i;
        }
        return -1;
    }

    @Override
    public void start(Stage primaryStage) throws Exception {
        primaryStage.setTitle("Parallel programming Visualisation Program");


        cells = FileReaderHelper.readFileAndFillArray();
        turns = new ArrayList<>();

        for(CellModel cell : cells){

            if(cell.getPosx() > maxCellX)
                maxCellX = cell.getPosx();
            if(cell.getPosy() > maxCellY);
            maxCellY = cell.getPosy();

            int tmp = getTurn(turns,cell.getLap());
            if(tmp != -1) {
                turns.get(tmp).addCell(cell);
            }
            else {
                turns.add(new TurnModel(cell));
            }
        }

        hBox=new HBox();
        vBox = new VBox();
        lbSuceptible = new Label("SUCEPTIBLE");
        lbSuceptible.setPadding(new Insets(10,10,10,10));
        lbSuceptible.setTextFill(Color.BLUE);
        lIncubation = new Label("INCUBATION");
        lIncubation.setPadding(new Insets(10,10,10,10));
        lIncubation.setTextFill(Color.PURPLE);
        lInfectious = new Label("INFECTIOUS");
        lInfectious.setPadding(new Insets(10,10,10,10));
        lInfectious.setTextFill(Color.RED);
        lIsolated = new Label("ISOLATED");
        lIsolated.setPadding(new Insets(10,10,10,10));
        lIsolated.setTextFill(Color.ORANGE);
        lImmunity = new Label("IMMUNITY");
        lImmunity.setPadding(new Insets(10,10,10,10));
        lImmunity.setTextFill(Color.GREEN);
        lDead = new Label("DEAD");
        lDead.setPadding(new Insets(10,10,10,10));
        lDead.setTextFill(Color.BLACK);
        turnNumber = new Label("Turn Number");
        turnNumber.setPadding(new Insets(10,10,10,10));
        slider = new Slider();
        slider.setPadding(new Insets(10,10,10,10));
        tvTurn = new TextField();
        tvTurn.setPadding(new Insets(10,10,10,10));
        tvTurn.setEditable(false);
        tvTurn.setText("0");

        slider.setMax(turns.size()-1);
        slider.setMin(0);
        slider.setBlockIncrement(1);

        slider.valueProperty().addListener((
                ObservableValue<? extends Number> ov,
                Number old_val, Number new_val) -> {
            tvTurn.setText(String.format("%d", new_val.intValue()));
            draw(new_val.intValue());
        });

        vBox.getChildren().add(lbSuceptible);
        vBox.getChildren().add(lIncubation);
        vBox.getChildren().add(lInfectious);
        vBox.getChildren().add(lIsolated);
        vBox.getChildren().add(lImmunity);
        vBox.getChildren().add(lDead);
        vBox.getChildren().add(turnNumber);
        vBox.getChildren().add(tvTurn);
        vBox.getChildren().add(slider);



        vBox.setPadding(new Insets(10,10,10,10));

        boardCanvas = new Canvas(600,600);
        hBox.getChildren().add(boardCanvas);
        hBox.getChildren().add(vBox);
        primaryStage.setScene(new Scene(hBox));


        primaryStage.show();
        draw(Integer.parseInt(tvTurn.getText()));
    }

    private void draw(int turn) {
        GraphicsContext context = boardCanvas.getGraphicsContext2D();

        double height = boardCanvas.getHeight();
        double width = boardCanvas.getWidth();

        // Clear the board
        context.clearRect(0, 0, width, height);

        cells = turns.get(turn).getCells();
        for (int x = 0; x <= maxCellX; x++) {
            for (int y = 0; y <= maxCellY; y++) {
                context.setStroke(Color.WHITE);
                context.strokeRect(width / maxCellX * x, height / maxCellY * y, width / maxCellX, height / maxCellY);
                for (CellModel cell : cells) {
                    if (cell.getPosx() == x && cell.getPosy() == y) {

                        switch (cell.status){
                            case "SUCEPTIBLE":
                                context.setFill(Color.BLUE);
                                break;
                            case "INCUBATION":
                                context.setFill(Color.PURPLE);
                                break;
                            case "INFECTIOUS":
                                context.setFill(Color.RED);
                                break;
                            case "ISOLATED":
                                context.setFill(Color.ORANGE);
                                break;
                            case "IMMUNITY":
                                context.setFill(Color.GREEN);
                                break;
                            case "DEATH":
                                context.setFill(Color.BLACK);
                                break;
                            default:
                                context.setFill(Color.WHITE);
                        }
                        context.fillRect(width / maxCellX * x, height / maxCellY * y, width / maxCellX, height / maxCellY);
                    }
                }
            }
        }
    }
}
