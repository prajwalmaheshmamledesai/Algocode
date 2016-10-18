package com.prajwal;

import java.io.StringWriter;
import java.util.HashMap;
import java.util.Map;

import freemarker.template.Configuration;
import freemarker.template.Template;
import spark.*;
import spark.http.matching.Halt;;

/**
 * Hello world!
 *
 */
public class App {
	public static void main(String[] args) {
		Configuration configuration = new Configuration();
		configuration.setClassForTemplateLoading(App.class, "/");

		Spark.get("/", new Route() {

			@Override
			public Object handle(Request arg0, Response arg1) throws Exception {
				StringWriter writer = new StringWriter();
				try {
					Template helloTemplate = configuration.getTemplate("hello.ftl");

					Map<String, Object> helloMap = new HashMap<String, Object>();
					helloMap.put("name", "Prajwal");

					helloTemplate.process(helloMap, writer);

					System.out.println(writer);

				} catch (Exception e) {

					e.printStackTrace(); // To change body of catch statement
											// use File |
											// Settings | File Templates.
				}

				return writer;
			}
		});

	}
}
