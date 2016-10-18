package com.prajwal;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;

import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.servlet.DefaultServlet;
import org.eclipse.jetty.servlet.ServletContextHandler;
import org.eclipse.jetty.servlet.ServletHolder;
import org.glassfish.jersey.server.ServerProperties;
import org.glassfish.jersey.servlet.ServletContainer;

import io.swagger.jaxrs.config.BeanConfig;
import io.swagger.jaxrs.config.DefaultJaxrsConfig;

//import com.wordnik.swagger.jaxrs.*;
//import com.wordnik.swagger.jaxrs.config.BeanConfig;

/**
 * Hello world!
 *
 */
public class App {

	private static class Bootstrap extends HttpServlet {
		@Override
		public void init(ServletConfig config) throws ServletException {
			super.init(config);
			BeanConfig beanConfig = new BeanConfig();
			beanConfig.setVersion("1.0.2");
			// beanConfig.set
			beanConfig.setSchemes(new String[] { "http" });
			beanConfig.setHost("localhost:8090");
			// beanConfig.setResourcePackage(arg0);
			beanConfig.setBasePath("/api");
			beanConfig.setResourcePackage("io.swagger.resources");
			beanConfig.setScan(true);
		}
	}

	// public static void main(String[] args) throws Exception {
	// ServletContextHandler context = new
	// ServletContextHandler(ServletContextHandler.SESSIONS);
	// context.setContextPath("/");
	//
	// Server jettyServer = new Server(2008);
	// jettyServer.setHandler(context);
	//
	// ServletHolder jerseyServlet =
	// context.addServlet(org.glassfish.jersey.servlet.ServletContainer.class,
	// "/*");
	// jerseyServlet.setInitOrder(0);
	//
	// // Tells the Jersey Servlet which REST service/class to load.
	// jerseyServlet.setInitParameter("jersey.config.server.provider.classnames",
	// EntryPoint.class.getCanonicalName());
	//
	//// ServletHolder swaggerServlet = new ServletHolder(new Bootstrap());
	//// // context.addServlet(swaggerServlet, "/*");
	////
	//// jettyServer.setHandler(context);
	//
	// try {
	// jettyServer.start();
	// jettyServer.join();
	// } catch (Exception ex) {
	// ex.printStackTrace();
	// }
	// }

	public static void main(String[] args) throws Exception {
		Server server = new Server(8010);
		ServletContextHandler context = new ServletContextHandler(ServletContextHandler.SESSIONS);
		context.setContextPath("/");
		server.setHandler(context);

		// Setup API resources
		ServletHolder apiServlet = context.addServlet(ServletContainer.class, "/api/*");
		apiServlet.setInitOrder(0);
		apiServlet.setInitParameter("com.sun.jersey.config.property.packages",
				"com.api.resources;io.swagger.jaxrs.json;io.swagger.jaxrs.listing");

		apiServlet.setInitParameter("jersey.config.server.provider.classnames", EntryPoint.class.getCanonicalName());
		// Setup Swagger servlet
		ServletHolder swaggerServlet = context.addServlet(DefaultJaxrsConfig.class, "/swagger-core");
		swaggerServlet.setInitOrder(1);
		swaggerServlet.setInitParameter("api.version", "1.0.0");
		
		
		server.start();
		server.join();
	}

}
