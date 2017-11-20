<?php
/*
UserSpice 4
An Open Source PHP User Management System
by the UserSpice Team at http://UserSpice.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//This file goes in your root directory
?>
<?php
require_once 'users/init.php';
require_once $abs_us_root.$us_url_root.'users/includes/header.php';
require_once $abs_us_root.$us_url_root.'users/includes/navigation.php';

$d = $db->query("SELECT * FROM temperature")->first();
$date = date("Y-m-d H:i:s");
$current = strtotime($date);


if(!empty($_POST)){
	if(!empty($_POST['led'])){
		$led = Input::get('led');
		mqtt(2,"usIN/ring",$led);
	}

	if(!empty($_POST['relay1on'])){
		mqtt(2,"usIN/relay1on",1);
	}

	if(!empty($_POST['relay1off'])){
		mqtt(2,"usIN/relay1off",1);
	}

	if(!empty($_POST['relay2on'])){
		mqtt(2,"usIN/relay2on",1);
	}

	if(!empty($_POST['relay2off'])){
		mqtt(2,"usIN/relay2off",1);
	}

}
?>

<?php //if (!securePage($_SERVER['PHP_SELF'])){die();} ?>

<div id="page-wrapper">
	<div class="container-fluid">
		<!-- Page Heading -->
		<div class="row">
			<div class="col-sm-6">
				<h1 class="page-header">
					Thanks for visiting my page!

				</h1>
				<h3>The temperature is <?=$d->temp?> and was reported
					<?php
					$stamp = strtotime($d->timestamp);
					$secs = $current - $stamp;
					$mins = $secs / 60;
					//$mins = $mins + 180; //timezone correction
					$mins = round($mins);
					if($mins > 20){
						echo "<font color='red'>";
					}
					echo $mins." mins ago</font>";
					?>

				</h3>
				<!-- Content goes here -->

				Flash the LED Ring
				<form class="" action="mqtt.php" method="post">
					<select class="" name="led">
						<option value="red">Red</option>
						<option value="green">Green</option>
						<option value="blue">Blue</option>
						<option value="rainbow">Rainbow</option>
					</select>
					<input type="submit" name="submit" value="submit">
				</form>
				<br><br>
				<form class="" action="mqtt.php" method="post">
					<input type="submit" name="relay1on" value="relay1on">
					<input type="submit" name="relay1off" value="relay1off">

					<input type="submit" name="relay2on" value="relay2on">
					<input type="submit" name="relay2off" value="relay2off">

				</form>
			</div>
		</div> <!-- /.row -->
	</div> <!-- /.container -->
</div> <!-- /.wrapper -->


<?php require_once $abs_us_root.$us_url_root.'users/includes/page_footer.php'; // the final html footer copyright row + the external js calls ?>

<!-- Place any per-page javascript here -->

<?php require_once $abs_us_root.$us_url_root.'users/includes/html_footer.php'; // currently just the closing /body and /html ?>
