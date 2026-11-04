import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.sql.*;

public class PopularCities extends HttpServlet {
    static final String DB_URL = "jdbc:mysql://localhost:3306/assignment_3";
    static final String USER = "root";
    static final String PASS = "q1w2e3r4!";

    public void init() throws ServletException {
        // Do required initialization
    }

    // Find out the top 6 most popular cities where people love to visit
    // If there is a tie, choose the smaller one with alphabetical order
    // For example, Shanghai is chosen over Taipei because S < T
    //
    // Hint: the city with most photos taken is the most popular one
    public void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {

        response.setContentType("text/html");
        PrintWriter out = response.getWriter();
        String title = "Database Result";

        String docType =
                "<!doctype html public \"-//w3c//dtd html 4.0 " + "transitional//en\">\n";

        out.println(docType +
                "<html>\n" +
                "<head><title>" + title + "</title></head>\n" +
                "<body bgcolor = \"#f0f0f0\">\n" +
                "<h1 align = \"center\">" + title + "</h1>\n");

        try {
            // Register JDBC driver
            Class.forName("com.mysql.jdbc.Driver");

            Connection conn = DriverManager.getConnection(DB_URL, USER, PASS);
            Statement stmt = conn.createStatement();
            // TODO: Implement your logic here!
            String query_popularCity = """
                    SELECT count(*) AS city_num, city
                    FROM photo
                    GROUP BY city
                    ORDER BY city_num DESC, city ASC
                    LIMIT 6;
                    """;


            ResultSet result = stmt.executeQuery(query_popularCity);

            int idx = 1;
            out.println("The cities with the 6 most uploads are... <br><br>");
            while(result.next())
            {
                int cityNum = result.getInt("city_num");
                String cityName = result.getString("city");

                out.println(idx + ". " + cityName + " uploaded " + cityNum + " times! <br>");
                idx++;
            }

        } catch (SQLException ex) {
            ex.printStackTrace();
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        }
    }
}
