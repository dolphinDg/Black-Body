library("plotrix")

make.show <- function() {

	cat("Choose temp_to_rgb.txt\n")
	t <- read.table(file=file.choose())
	draw.plot <- function(i) {
		plot(c(-1, 1), c(-1,1), type="n",xlab="",ylab="",bty="n",xaxt="n",yaxt="n")
		draw.circle(0, 0, col = rgb(t[i,2], t[i,3], t[i,4], maxColorValue=255),radius=0.8)
		if (abs(t[i,1] - 5800) < 300)
			text(x=0,y=0.1,"Sun")
		if (abs(t[i,1] - 10000) < 300)
			text(x=0,y=0.1,"Sirius")
		text(x=0,y=0, paste("T = ",t[i,1],"K"))
	}
	
	for (i in 1:nrow(t)) {
		draw.plot(i)
		Sys.sleep(0.2)
	}
}

make.pictures <- function(folder) {
		
	#folder: path to folder where images and gif will be created
		#format: /Users/username/Documents/BlackBody/
		#Warning: last forward slash is necessary!
		
	extend <- function(a) {
		n<-length(strsplit(as.character(a),split="")[[1]])
		zeroes<-6-n
		zeroes<-paste(rep(0,times=zeroes),collapse="")
		res<-paste(zeroes,a,sep="")
		return(res)
	 }
	
	cat("Choose temp_to_rgb.txt\n")
	t <- read.table(file=file.choose())
	draw.plot <- function(i) {
		plot(c(-1, 1), c(-1,1), type="n", xlab="", ylab="", bty="n", xaxt="n", yaxt="n")
		draw.circle(0, 0, col = rgb(t[i,2], t[i,3], t[i,4], maxColorValue=255), radius = 0.8)
		if (abs(t[i,1] - 5800) < 300)
			text(x=0,y=0.1,"Sun")
		if (abs(t[i,1] - 10000) < 300)
			text(x=0,y=0.1,"Sirius")
		text(x=0,y=0, paste("T = ",t[i,1],"K"))
	}
	
	progress_bar <- txtProgressBar(0, nrow(t), style = 1, char = "=")

	for (i in 1:nrow(t)) {
		png(filename=paste(folder, "star_at", extend(t[i,1]), ".png", sep=""), width=400,height=400)
		draw.plot(i)
		dev.off()
		Sys.sleep(0.2)
		setTxtProgressBar(progress_bar, value = i);
	}
	cat("\nFinished\n")
}
