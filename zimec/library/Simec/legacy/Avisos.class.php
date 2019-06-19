<?php


class Avisos{
	
	public function __construct(){
		
		
		
		
	}
	/*
	 * Parametros do array:
	 * array(
	 * 		array( 'titulo' => "titulo do aviso", 'texto' => 'texto do aviso' ),
	 *		array( 'titulo' => "titulo do aviso", 'texto' => 'texto do aviso' ),
	 *		..................
	 * )
	 * 
	 * */
	function getAvisos($arrAvisos = array())
	{
		
		$arrayCores = array('#FF6D3F', '#FF9B00', '#FFDA00', '#95D641', '#1CE8B5','#3FC3FF', '#B8C4C9' );
		
		//die((string));
		if(count($arrAvisos) > 0 )
		{
			$strRetorno = "<div id=\"dialog_vigencia\" style=\"display: none;\">
							";
			$i = 0; 
			if(count($arrAvisos) == 1)
			{
				$md = 8;
				$width = '95%';
			}
			if(count($arrAvisos) == 2)
			{
				$md = 4;
				$width = '45%';
			}
			if(count($arrAvisos) == 3)
			{
				$md = 3;
				$width = '30%';
			}
			if(count($arrAvisos) >= 4)
			{
				$mdEspaço = '1';
				$md = 2;
				$width = '22%';
			}
			foreach($arrAvisos as $k => $aviso)
			{
				$i++;
				
				// Caso seja o quarto aviso ou primeira volta adiciona linha
				if( ($i == 1)  )
				{
					$strRetorno .= "
								<div class=\"row\" style=\" margin-top:30px;\">
									
									";
				}
				// Adiciona conteúdo
				$cor = $arrayCores[rand ( 0 , 6 )];
				$titulo = strtoupper($aviso['titulo']);
				$strRetorno .=		"
									<div class=\"col col-md-1\" style=\"width:0px !important;\"></div>
									
									<div class=\"col col-md-{$md}\" style=\"padding:0; width: {$width} !important; min-height:250px !important;\">
										<div  class=\"panel panel-warning\">
									
										    <div class=\"panel-heading\" style=\" font-family: 'Arial',serif; font-size: 14px; color:black; border:0px !important; background-color:{$cor} !important ;\">
									
 										        <h3 class=\"panel-title\" style=\"  font-family: 'Arial',serif; font-size: 14px; \"><b>{$titulo}</b></h3>
									
										    </div>
									
										    <div  class=\"panel-body\"  style=\"   font-family: 'Arial',serif; font-size: 14px;  background-color:{$cor}; min-height:200px\"><span style=\"  font-family: font-family: 'Arial',serif; font-size: 14px;\">{$aviso['texto']}</span> <br><br></div>
									
										</div>
									</div>";
				// Caso seja par ou ultima volta fecha 
				if(  ( ($i % 4) == 0   ) && ($i != count($arrAvisos) ) )
				{
					$strRetorno .= "
									
								</div>
								<div class=\"row\" style=\"margin-top:30px;\">
									";
				}
				else if( $i == count($arrAvisos)  )
				{
					$strRetorno .= "
					
								</div>
									";
				}
			}	
			
			$strRetorno .= "
						   </div>";
			
			
			$strRetorno .= "
					<style>
					
					.panel-body a  {
						color: #15c;
					  text-decoration: underline;
					}
					
					
					.panel-warning{
						
 						box-shadow: 1px 4px 2px #DDDDDD;
						-moz-box-shadow: 1px 4px 2px #DDDDDD;
						-moz-box-shadow: 1px 4px 2px #DDDDDD;
						-webkit-border-bottom-right-radius: 7px !important;
						-moz-border-radius-bottomright: 7px !important;
						border-bottom-right-radius: 7px !important;
					}
					
					.panel-body{
						
 						box-shadow: 2px 4px 4px #DDDDDD;
						-moz-box-shadow: 2px 4px 4px #DDDDDD;
						-moz-box-shadow: 2px 4px 4px #DDDDDD;
						-webkit-border-bottom-right-radius: 7px !important;
						-moz-border-radius-bottomright: 7px !important;
						border-bottom-right-radius: 7px !important;
					}
					
					.ui-widget-overlay{
			        
			        	opacity: 0.9 !important;
			        
			   		 }	
 					.ui-dialog-titlebar {
						display: none !important; 
					}		
					</style>
		 <script type=\"text/javascript\">
		        jQuery(function(){
		                    jQuery(\"#dialog_vigencia\").dialog({
		                        modal: true,
		                        width: '90%',
								height: 700,
		                        open: function(){
						            jQuery('.ui-widget-overlay').bind('click',function(){
						                jQuery('#dialog_vigencia').dialog('close');
						            })
						        },
		                        buttons: {
		                            Fechar: function() {
		                                jQuery(\"#dialog_detalhe_processo\").html('');
		                                jQuery( this ).dialog( \"close\" );
		                            }
		                        }
		                    });
					 
		        });
					
				
					
		    </script>
			<style>
				.ui-dialog-titlebar {
						display: block; 
					}
			</style>
					";
			echo $strRetorno;
		}
	}
	
	
}