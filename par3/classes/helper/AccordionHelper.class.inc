<?php

/**
 * Class Par3_Helper_AccordionHelper
 *
 * Criado por: Daniel Mileski Marconi
 *
 * Componente accordion bootstrap
 * Utilizado em Execução e Acompanhamento/contrato
 *
 */

class Par3_Helper_AccordionHelper
{

    private static $contadorTeste = 0;

    public function __construct(){

    }

    /**
     * A classe precisa ser instanciada, se ouver mais de um accordion na página, para que o contador de ids funcione.
     *
     * *Importante*
     * Definir a requisicao desejada através do parâmetro $config, com o conteúdo a ser mostrado dentro dos accordions
     *
     * @param $arrCabecalhos - Array de accordions
     *        ['descricao'] = título do header
     *        ['id'] = id do header usado para carregar o conteúdo via ajax (chama a config 'req')
     *        ['descitem'] = subtítulo (opcional)
     *        ['qtditens'] = quantidade. Acompanha a o subtítulo. (opcional) Ex: Quantidade de itens sem contrato vinculado: 10
     *                                                                ['qtddescricao']:['qtditens']
     *
     * @param $config
     *        ['checkbox'] => bool true|false(default)
     *                      true: Coloca a opção "selecionar todos": seleciona todos os checkboxs dentro do painel aberto.
     *        ['recarregarDiv'] => bool true(default)|false
     *                      true: toda vez que clicar no título, a div #result será recarregada pelo ajax
     *                      false: se a div #result já estiver preenchida, apenas abre o accordion, sem requisição. A lógica para atualizar deverá ser feita por fora (ou ao recarregar a página).
     *        ['req'] => string nome da requisicao, caso queira mudar
     *
     * @return void | string
     */

    public static function listagemAccordion($arrCabecalhos, $config = array()){

        $req = $config['req'] ? $config['req'] : 'carregaListaAccordion';
        $mensagemTooltip = $config['msgtooltip'] ?: 'Clique para visualizar mais informações';

        static $test = 0;
        $html = '';

        //Ids precisam ser diferentes, caso haja mais de um accordion na página.
        if(!$_SESSION['contadorAccordion']){
            $_SESSION['contadorAccordion'] = 0;
        }
        if(!$_SESSION['contadorTabs']){
            $_SESSION['contadorTabs'] = 0;
        }

        $contadorAccordion = (int) $_SESSION['contadorAccordion'];
        $contadorTabs = (int) $_SESSION['contadorTabs'];

        $recarregarDiv = $config['recarregarDiv'] === false ? 'false' : 'true';

        if($test == 0){
            $html .= '<style>
                        .panel-heading:hover {
                            background-color: #eaeaea;
                        }
                     </style>';
        }

        $html .= '<div class="panel-group" id="accordion'. $contadorAccordion .'" role="tablist" aria-multiselectable="true">';
        $html .= '<input type="hidden" id="requestemandamento" value="false">'; //evita muitos cliques/requisições ao mesmo tempo
        $html .= '<input type="hidden" id="recarregarDiv" value="'. $recarregarDiv .'">'; //evita muitos cliques/requisições ao mesmo tempo

        foreach ($arrCabecalhos as $key => $cabecalho){

            //subtítulo
            if($cabecalho['qtditens'] || $cabecalho['descitem']){
                $subtitulo = '<span class="badge">' . $cabecalho['descitem'] . $cabecalho['qtditens'] . '</span>';
            }else{
                $subtitulo = '';
            }

            //Iniciar página com o primeiro accordion aberto.
//            $classIn = $key == 0 ? 'in ' : '';
//            $classCollapsed = $key == 0 ? '' : 'collapsed';
//            $ariaExpanded = $key == 0 ? 'true' : 'false';

            $html .= '<div class="panel panel-default" id="panel'. $contadorTabs .'">';

            $html .= '<div class="panel-heading" role="tab" id="heading'. $contadorTabs .'" data-toggle="tooltip" title="' . $mensagemTooltip . '" data-placement="top">
                        <h4 class="panel-title">
                            <a role="button" class="'. $req .' collapsed" data-id="' . $cabecalho['id'] . '" data-toggle="collapse" data-parent="#accordion'. $contadorAccordion . '" href="#collapse'. $contadorTabs .'" aria-expanded="false" aria-controls="collapse'. $contadorTabs .'" style="display:block">
                                ' . $cabecalho['descricao'];

            $html .= '<i style="float:right;color: #7f7f7f;" class="more-less glyphicon glyphicon-plus btn-lg"></i>';

            if($subtitulo == '') $html .= '<br>';

            $html .= '<br><br>
                                ' . $subtitulo . '
                            </a>
                        </h4>
                    </div>';

            $html .= '<div id="collapse'. $contadorTabs .'" class="panel-collapse collapse role="tabpanel" aria-labelledby="heading'. $contadorTabs .'">';
            $html .= '<div class="panel-body">'; //conteúdo

            //div contendo outras opções
            if($config['checkbox'] === true){
                $html .= '<div>
                            <label class="form-check-label" style="width: 100%;height: 100%;cursor: pointer;margin:0;padding:4px;">
                              <input class="form-check-input checkall" type="checkbox" data-idaccordion="'. $contadorAccordion .'" data-idpanel="'. $contadorTabs .'" onclick="'.$req.'.selecionarTodos(this.dataset.idaccordion, this.dataset.idpanel, this.checked)"> 
                              Selecionar Todos
                            </label>
                          </div>';
            }

            //div que irá conter a lista
            $html .= '<div id="result'.$req.$cabecalho['id'].'" class="resultAjax"></div>';

            $html .= '</div></div>'; //fim conteúdo

            $html .= '</div>'; // fim panel-default

            $contadorTabs++;
        }

        $html .= '</div>';

        $html .= '<script>


                    
                  var '.$req.' = (function(){ 
                  
                  return {
                    
                      selecionarTodos: function(idaccordion, idpanel, checked){
                          if(checked){
                            $("#accordion"+idaccordion+" #panel" + idpanel + " :checkbox").prop("checked",true);
                          }else{
                            $("#accordion"+idaccordion+" #panel" + idpanel + " :checkbox").prop("checked",false);
                          }
                      },
                      
                      carregaListaAccordion: function(id){
                        $("#requestemandamento").val("true");
                        
                        var req = "' . $req . '";
                        
                        $.post(window.location.href, {requisicao: req, id: id}, function (resp) {
                            $(".checkall").prop("checked",false);
                            $("#result"+req+id).html(resp);
                            $("#requestemandamento").val("false");
                        });
                      }
                  
                  };
                
                  })();     

                    $(".' . $req . '[data-id]").click(function(){
                       var id = $(this).attr("data-id"); 
                       var req = "' . $req . '";
                       if($(this).hasClass("collapsed") && $("#requestemandamento").val() === "false"){
                           if($("#recarregarDiv").val() === "false"){
                               if($("#result"+req+id).html() === ""){ //executa apenas se estiver vazio
                                   '.$req.'.carregaListaAccordion(id);  
                               }
                           }else{
                               '.$req.'carregaListaAccordion(id); 
                           }
                       }
                    }); 
                   
                  
                   //'.$req.'.carregaListaAccordion('.$arrCabecalhos[0]['id'].'); 
                  </script>';

        $contadorAccordion++;
        $_SESSION['contadorAccordion'] = $contadorAccordion;
        $_SESSION['contadorTabs'] = $contadorTabs;
        $test++;
        echo $html;
    }


}