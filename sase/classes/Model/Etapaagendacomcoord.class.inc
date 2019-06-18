<?php
    require_once APPRAIZ . 'includes/library/simec/Listagem.php';

    /**
     * Classe de mapeamento da entidade sase.etapaagendacomcoord.
     *
     * @version $Id$
     * @since 2016.08.15
     *
     * Sase_Model_Etapaagendacomcoord: sem descricao
     *
     * @package Sase\Model
     * @uses Simec\Db\Modelo
     * @author Luciano Fernandes Ribeiro <luciano.ribeiro@mec.gov.br>
     */
    class Sase_Model_Etapaagendacomcoord extends Modelo{
        /**
         * @var string Nome da tabela mapeada.
         */
        protected $stNomeTabela = 'sase.etapaagendacomcoord';

        /**
         * @var string[] Chave primaria.
         */
        protected $arChavePrimaria = array(
            'eacid',
        );

        /**
         * @var mixed[] Chaves estrangeiras.
         */
        protected $arChaveEstrangeira = array(
        );

        /**
         * @var mixed[] Atributos da tabela.
         */
        protected $arAtributos = array(
            'eacid' => null,
            'eacdsc' => null,
            'accstatus' => null,
            'accobservacao' => null,
            'accprazo' => null,
            'accresponsavel' => null,
            'accacao' => null,
            'eacid' => null,
            'accid' => null,
        );

        /**
         * Validators.
         *
         * @param mixed[] $dados
         * @return mixed[]
         */
        public function getCamposValidacao($dados = array()){
            return array(
                'eacid' => array('Digits'),
                'eacdsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 60))),
                'accstatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
            );
        }

        public function antesSalvar(){
            // -- Implemente suas transformações de dados aqui

            // -- Por padrão, o método sempre retorna true
            return parent::antesSalvar();
        }

        public function atualizaComboMunicipio( $dados ){
            $uf = $dados['estuf'];
            if( $uf != '' ){
                $sql = "
                    SELECT  muncod AS codigo,
                            mundescricao AS descricao
                    FROM territorios.municipio
                    WHERE estuf = '{$uf}'
                ";                    
                return $this->monta_combo("muncod", $sql, 'S', 'Selecione...', '', '', '', '436', 'S', 'muncod', false, $muncod, 'Município', '', 'chosen-select');
                die();
            }
        }


        public function listagemMunicipiosAgenda( $dados ){
           
        $perfis = pegaPerfilGeral($_SESSION['usucpf']);
        $usucpf = $_SESSION['usucpf'];
        $saseTec = PFLCOD_SASE_TECNICO;


        if ($dados['estuf'] != '') {
            $and[] = "est.estuf = '{$dados['estuf']}'";
        }

        if ($dados['muncod'] != '') {
            $and[] = "mun.muncod = '{$dados['muncod']}'";
        }

        if ($dados['etapas'] == 'S') {
            $and[] = "accdatainclusao IS NOT NULL"; 
                   
        } elseif ($dados['etapas'] == 'N') {
            $and[] = "accdatainclusao IS NULL ";
        }
        if ($and[0] != '') {
            $_and = ' AND ' . implode(' AND ', $and);
        }

        if (in_array(PFLCOD_SASE_SUPER_USUARIO, $perfis) || in_array(PFLCOD_SASE_ADMINISTRADOR, $perfis)) {
            //carrega os municípios, quando o usuário tem o perfil super ou adm, carrega todos os municípios que são trabalhados pelo perfil SASE_TECNICO
            $where = "INNER JOIN sase.usuarioresponsabilidade usu ON mun.muncod = usu.muncod"
                    . " WHERE rpustatus = 'A' AND pflcod = {$saseTec} {$_and}";
        } elseif (in_array(PFLCOD_SASE_EXECUTIVO, $perfis) || in_array(PFLCOD_SASE_SUPERVISOR_GERAL, $perfis)) {
            //carrega os municípios atribuídos ao usuário técnico logado.
            $with = "WITH temp_estado AS (select estuf from sase.usuarioresponsabilidade WHERE rpustatus = 'A' AND usucpf = '{$_SESSION['usucpf']}')";
            $where = "WHERE est.estuf IN (select estuf from temp_estado) {$_and}";
        }elseif (in_array(PFLCOD_SASE_TECNICO, $perfis)) {
            //carrega os municípios atribuídos ao usuário técnico logado.
            $with = "WITH temp_estado AS (select muncod from sase.usuarioresponsabilidade WHERE rpustatus = 'A' AND usucpf = '{$_SESSION['usucpf']}')";
            $where = "WHERE mun.muncod IN (select muncod from temp_estado) {$_and}";
        }

        $SQL = <<<DML
        
            {$with}
            SELECT DISTINCT
                    mun.muncod AS acao,
                    est.estuf||' - '||estdescricao as estado,
                    mun.mundescricao,
                    CASE WHEN accdatainclusao IS NOT NULL 
                        AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 1 and accstatus = 'A' AND acc.muncod = mun.muncod AND ( accacao != '' AND accresponsavel != '' AND accprazo IS NOT NULL) ) )
                    THEN 'S'
                    ELSE 'N'
                    END AS preenchido
            FROM territorios.municipio mun
            INNER JOIN territorios.estado est on mun.estuf = est.estuf
            LEFT JOIN sase.agendacomissaocoordenadora acc  on acc.muncod = mun.muncod
            {$where} ORDER BY 2, 1;      
        
        
        
        
DML;

        $cabecalho = array('Estado', 'Município', 'Agenda Preenchida');
        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($SQL);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo('preenchido', 'pegaImagem');
        $listagem->addAcao('edit', 'editar_agenda');

        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

        public function getSqlTrabalhosAgendados($muncod, $etapa){
        $sql = "
                    SELECT  accid,
                            eacid,
                            acc.muncod,
                            trim(accacao) AS accacao,
                            trim(accresponsavel) AS accresponsavel,
                            to_char(accprazo, 'DD/MM/YYYY') AS accprazo,
                            trim(accobservacao) AS accobservacao,
                            to_char(accconcluida, 'DD/MM/YYYY') AS accconcluida,                             
                            CAST(accconcluida AS DATE) + INTERVAL '30 DAYS' AS datalimit,
                            to_char(accdatainclusao, 'YYYY') AS accdatainclusao,
                             accdatainclusao as datadesc
                    FROM sase.agendacomissaocoordenadora AS acc
                    inner join territorios.municipio AS mun on acc.muncod = mun.muncod
                    inner join seguranca.usuario AS  usu on acc.usucpf = usu.usucpf

                    WHERE accstatus = 'A' AND acc.muncod = '{$muncod}' AND eacid = {$etapa}
                    ORDER BY accdatainclusao DESC  
                    
                      
                ";

        return $this->carregar($sql);
    }

        public function ListaTrabalhosAgendados( $muncod, $etapa ){
            $perfis  = pegaPerfilGeral( $_SESSION['usucpf'] );
            if( $muncod > 0 ){
                $res = $this->getSqlTrabalhosAgendados($muncod, $etapa);
            }

            $contN = 0;
            if (is_array($res)){
                $contN = count($res);
            }


            $html = "
                
                
                <style>
                    .campoData{
                        width: 100px !important;
                    }
                    .spanObs{
                        font-size: 10px;
                    }
                    th{
                        text-align: center;
                    }
                </style>

                <form name=\"formulario_{$etapa}\" id=\"formulario_{$etapa}\" class=\"form-horizontal\" method=\"POST\">
                    <input type=\"hidden\" id=\"request\" name=\"request\" value=\"\"/>
                    <input type=\"hidden\" id=\"eacid\" name=\"eacid\" value=\"{$etapa}\"/>
                    <input type=\"hidden\" id=\"excluir_accid\" name=\"excluir_accid\" value=\"\"/>
                    <input type=\"hidden\" id=\"muncod\" name=\"muncod\" value=\"{$muncod}\"/>

                    <div id=\"listagem\" style=\"background: #ffffff !important;overflow-y: scroll;max-height: 400px\">
                        <input type=\"hidden\" id=\"contN\" name=\"contN\" value=\"{$contN}\"/>
                        <table name=\"tabAtvDes\" class=\"table table-bordered table-hover table-condensed\" border=\"0\">
                        <thead>
                            <tr>
                                
                                <th width=\"340\">Ação</th>
                                <th width=\"240\">Responsável</th>
                                <th width=\"140\">Previsão de Execução</th>
                                <th>Observações</th>
                                <th width=\"140\">Concluída em</th>
                                <th width=\"140\">Ações</th>
                            </tr>
                        </thead>
                        <tbody>
            ";

            $i = 1;
            if( $res[0]['accid'] > 0 ){
                foreach( $res as $r ){

                    $r['accacao']        = stripslashes($r['accacao']);
                    $r['accresponsavel'] = stripslashes($r['accresponsavel']);
                    $r['accobservacao']  = stripslashes($r['accobservacao']);
                    $html .= "
                        <tr>
                            
                            <td>
                                <textarea readonly=\"readonly\" 
                                          class=\"form-control frm-disabled{$r['accid']} frm-obrigatorio-{$etapa}\" 
                                          id=\"accacao{$i}\" 
                                          maxlength=\"1000\" 
                                          name=\"accacao[]\" 
                                          rows=\"2\">{$r['accacao']}</textarea>
                            </td>
                            <td>
                                <textarea readonly=\"readonly\"
                                          class=\"form-control frm-disabled{$r['accid']} frm-obrigatorio-{$etapa}\" 
                                          id=\"accresponsavel{$i}\"
                                          maxlength=\"300\"
                                          name=\"accresponsavel[]\"
                                          rows=\"2\">{$r['accresponsavel']}</textarea>
                            </td>
                            <td>
                                <center>
                                    <input readonly=\"readonly\"
                                           id=\"accprazo_{$etapa}_{$i}\"
                                           name=\"accprazo_{$etapa}[]\"
                                           type=\"text\"
                                           class=\"campoData form-control frm-disabled{$r['accid']} frm-obrigatorio-{$etapa}\"
                                           value=\"{$r['accprazo']}\" placeholder=\"\">
                                </center>
                            </td>
                            <td>
                                <textarea readonly=\"readonly\"
                                          class=\"form-control frm-disabled{$r['accid']}\" 
                                          id=\"accobservacao{$i}\"
                                          maxlength=\"1000\"
                                          name=\"accobservacao[]\" rows=\"2\">{$r['accobservacao']}</textarea>
                            </td>
                             <td>
                                <center>
                                    <input readonly=\"readonly\" id=\"aceconcluida_{$etapa}_{$i}\" name=\"aceconcluida_{$etapa}[]\"
                                           type=\"text\"
                                           class=\"campoData form-control frm-disabled{$r['accid']} frm-obrigatorio-{$etapa}\" value=\"{$r['accconcluida']}\" >
                                </center>
                            </td>
                            
";
                    $ano_passado =  (date("Y")-1);
                    if(in_array( PFLCOD_SASE_TECNICO, $perfis) || in_array( PFLCOD_SASE_SUPERVISOR_GERAL, $perfis) || in_array( PFLCOD_SASE_EXECUTIVO, $perfis)) {

                       $r['datalimit'] = date("Y-m-d", strtotime($r['datalimit']));

                          if(  strtotime(date("Y-m-d")) > strtotime($r['datalimit'])  &&  $r['datalimit'] != '1969-12-31'   ){

                          } else if (date("Y") ==  $r['accdatainclusao']) {
                            $html .= " <td style=\"vertical-align: middle;\">
                                <center>
                                    <button title=\"Edtar\" class=\"btn btn-warning btnExcluir{$r['accid']}\" type=\"button\"
                                            data-toggle=\"modal\" data-target=\"#edtarAtividade_{$r['accid']}\"><span class=\"glyphicon glyphicon-pencil\"></span></button>
                                    <button title=\"Excluir\" class=\"btn btn-danger btnExcluir{$r['accid']}\" type=\"button\"
                                            onclick=\"excluirAtividade({$r['accid']})\"><span class=\"glyphicon glyphicon-trash\"></span></button>
                                                
                                    <input type=\"hidden\" name=\"accid[]\" id=\"accid{$i}\" value=\"{$r['accid']}\" />
                                    <input type=\"hidden\" name=\"reg[]\" id=\"reg{$i}\" value=\"{$i}\"/>
                                </center>    
                            </td>";
                        }else if ($r['accconcluida'] == '' &&   date("Y")  ==  $r['accdatainclusao']) {
                              $html .= " <td style=\"vertical-align: middle;\">
                                <center>
                                    <button title=\"Edtar\" class=\"btn btn-warning btnExcluir{$r['accid']}\" type=\"button\"
                                            data-toggle=\"modal\" data-target=\"#edtarAtividade_{$r['accid']}\"><span class=\"glyphicon glyphicon-pencil\"></span></button>
                                    <button title=\"Excluir\" class=\"btn btn-danger btnExcluir{$r['accid']}\" type=\"button\"
                                            onclick=\"excluirAtividade({$r['accid']})\"><span class=\"glyphicon glyphicon-trash\"></span></button>
                                                
                                    <input type=\"hidden\" name=\"accid[]\" id=\"accid{$i}\" value=\"{$r['accid']}\" />
                                    <input type=\"hidden\" name=\"reg[]\" id=\"reg{$i}\" value=\"{$i}\"/>
                                </center>    
                            </td>";
                          }else  if ($r['accconcluida'] == NULL && $ano_passado == $r['accdatainclusao']) {
                              $html .= " <td style=\"vertical-align: middle;\">
                                <center>
                                    <button title=\"Edtar\" class=\"btn btn-warning btnExcluir{$r['accid']}\" type=\"button\"
                                            data-toggle=\"modal\" data-target=\"#edtarAtividade_{$r['accid']}\"><span class=\"glyphicon glyphicon-pencil\"></span></button>
                                    <input type=\"hidden\" name=\"accid[]\" id=\"accid{$i}\" value=\"{$r['accid']}\" />
                                    <input type=\"hidden\" name=\"reg[]\" id=\"reg{$i}\" value=\"{$i}\"/>
                                </center>    
                            </td>";
                          }else  if (strtotime(date("Y-m-d")) < strtotime($r['datalimit'])) {
                              $html .= " <td style=\"vertical-align: middle;\">
                                <center>
                                    <button title=\"Edtar\" class=\"btn btn-warning btnExcluir{$r['accid']}\" type=\"button\"
                                            data-toggle=\"modal\" data-target=\"#edtarAtividade_{$r['accid']}\"><span class=\"glyphicon glyphicon-pencil\"></span></button>
                                    <input type=\"hidden\" name=\"accid[]\" id=\"accid{$i}\" value=\"{$r['accid']}\" />
                                    <input type=\"hidden\" name=\"reg[]\" id=\"reg{$i}\" value=\"{$i}\"/>
                                </center>    
                            </td>";
                          }


                    }else if(in_array( PFLCOD_SASE_ADMINISTRADOR, $perfis)) {

                        $html .= " <td style=\"vertical-align: middle;\">
                                <center>
                                    <button title=\"Edtar\" class=\"btn btn-warning btnExcluir{$r['accid']}\" type=\"button\"
                                            data-toggle=\"modal\" data-target=\"#edtarAtividade_{$r['accid']}\"><span class=\"glyphicon glyphicon-pencil\"></span></button>
                                    <button title=\"Excluir\" class=\"btn btn-danger btnExcluir{$r['accid']}\" type=\"button\"
                                            onclick=\"excluirAtividade({$r['accid']})\"><span class=\"glyphicon glyphicon-trash\"></span></button>
                                                
                                    <input type=\"hidden\" name=\"accid[]\" id=\"accid{$i}\" value=\"{$r['accid']}\" />
                                    <input type=\"hidden\" name=\"reg[]\" id=\"reg{$i}\" value=\"{$i}\"/>
                                </center>    
                            </td>";
                    }else{

                    }



                    //$select


                     $html .= "<div class=\"modal fade\" id=\"edtarAtividade_{$r['accid']}\" tabindex=\"-1\" role=\"dialog\" aria-labelledby=\"edtarAtividade_{$r['accid']}\">
                                <div class=\"modal-dialog  modal-xl\" role=\"document\">
                                    <div class=\"modal-content\">
                                        <div class=\"modal-header\">
                                            <button type=\"button\" class=\"close\" data-dismiss=\"modal\" aria-label=\"Close\"><span aria-hidden=\"true\">&times;</span></button>
                                            <h4 class=\"modal-title text-center\" id=\"myModalLabel\" >EDITAR DA AÇÃO DA ETAPA</h4>
                                        </div>
                                        <div class=\"modal-body\">
                                            <form name=\"formulario_edit_{$r['accid']}\" id=\"formulario_edit_{$r['accid']}\" class=\"form-horizontal\" method=\"POST\">
                                                <div class=\"form-group\">
                                                    <label class=\"col-md-2 control-label\" for=\"etapa\">Etapa:</label>
                                                    <div class=\"col-md-10\">
                                                        <select id=\"etapa_{$r['accid']}\" name=\"etapa\" class=\"form-control\"   ".(in_array(PFLCOD_SASE_TECNICO, $perfis)|| in_array( PFLCOD_SASE_SUPERVISOR_GERAL, $perfis)|| in_array( PFLCOD_SASE_EXECUTIVO, $perfis) ? ($r['accdatainclusao'] == $ano_passado ? 'disabled' : '') : '').">
                                                            <option value=\"0\" >Selecione</option>
                                                            <option value=\"1\" ".($etapa == '1' ? 'selected' : '') ." > I.  ETAPA - Organizar o Trabalho</option>
                                                            <option value=\"2\" ".($etapa == '2' ? 'selected' : '') ." > II. ETAPA - Estudar o Plano</option>
                                                            <option value=\"3\" ".($etapa == '3' ? 'selected' : '') ." > III. ETAPA - Monitorar continuamente as metas e estratégias</option>
                                                            <option value=\"4\" ".($etapa == '4' ? 'selected' : '') ." > IV. ETAPA - Avaliar periodicamente o plano</option>
                                                        </select>
                                                    </div>
                                                </div>
                                                
                                                <input type=\"hidden\" id=\"muncod_{$r['accid']}\" name=\"muncod_{$r['accid']}\" value=\"{$_GET['muncod']}\"/>
                                                <input type=\"hidden\" id=\"accid_{$r['accid']}\" name=\"accid_{$r['accid']}\" value=\"{$r['accid']}\"/>
                                                
                                                <div class=\"form-group\">
                                                    <label class=\"col-md-2 control-label\" for=\"accacao\">Ação:</label>
                                                    <div class=\"col-md-10\">
                                                        <textarea class=\"form-control\" cols=\"40\" id=\"accacao_{$r['accid']}\"  name=\"accacao_{$r['accid']}\" ".(in_array(PFLCOD_SASE_TECNICO, $perfis)|| in_array( PFLCOD_SASE_SUPERVISOR_GERAL, $perfis)|| in_array( PFLCOD_SASE_EXECUTIVO, $perfis) ? ($r['accdatainclusao'] == $ano_passado ? 'disabled' : '') : '').">{$r['accacao']}</textarea>
                                                    </div>
                                                </div>
                                                
                            
                                                <div class=\"form-group\">
                                                    <label class=\"col-md-2 control-label\" for=\"accresponsavel\">Responsável:</label>
                                                    <div class=\"col-md-10\">
                                                        <textarea class=\"form-control\" id=\"accresponsavel_{$r['accid']}\" name=\"accresponsavel_{$r['accid']}\" ".(in_array(PFLCOD_SASE_TECNICO, $perfis)|| in_array( PFLCOD_SASE_SUPERVISOR_GERAL, $perfis)|| in_array( PFLCOD_SASE_EXECUTIVO, $perfis)   ? ($r['accdatainclusao'] == $ano_passado ? 'disabled' : '') : '').">{$r['accresponsavel']}</textarea>
                                                    </div>
                                                </div>
                            
                            
                                                <div class=\"form-group\">
                                                    <label class=\"col-md-2 control-label\" for=\"accprazo\">Previsão de Execução:</label>
                                                    <div class=\"col-md-10\">
                                                        <input id=\"accprazo_{$r['accid']}\" maxlength=\"0\" name=\"accprazo_{$r['accid']}\" width=\"200px\" type=\"text\" value=\"{$r['accprazo']}\" class=\"form-control  campoData2 input-md\" ".(in_array(PFLCOD_SASE_TECNICO, $perfis)|| in_array( PFLCOD_SASE_SUPERVISOR_GERAL, $perfis)  || in_array( PFLCOD_SASE_EXECUTIVO, $perfis)? ($r['accdatainclusao'] == $ano_passado ? 'disabled' : '') : '').">
                            
                                                    </div>
                                                </div>
                            
                            
                                                <div class=\"form-group\">
                                                    <label class=\"col-md-2 control-label\" for=\"accobservacao\">Observações:</label>
                                                    <div class=\"col-md-10\">
                                                        <textarea class=\"form-control\" id=\"accobservacao_{$r['accid']}\" name=\"accobservacao_{$r['accid']}\" ".(in_array(PFLCOD_SASE_TECNICO, $perfis)|| in_array( PFLCOD_SASE_SUPERVISOR_GERAL, $perfis) || in_array( PFLCOD_SASE_EXECUTIVO, $perfis)  ? ($r['accdatainclusao'] == $ano_passado ? 'disabled' : '') : '').">{$r['accobservacao']}</textarea>
                                                    </div>
                                                </div>
                                                
                                                <div class=\"form-group\">
                                                           <label class=\"col-md-2 control-label\" for=\"accconcluida\">Concluída em:</label>
                                                           <div class=\"col-md-10\">
                                                            <input id=\"accconcluida_{$r['accid']}\" maxlength=\"0\" name=\"accconcluida_{$r['accid']}\" width=\"200px\" type=\"text\" value=\"{$r['accconcluida']}\" class=\"form-control  campoData2 input-md\">
                                                    </div>
                                                </div>
                            
                            
                                        </div>
                                        <div class=\"modal-footer\">
                                            <button type=\"button\" class=\"btn btn-default\"  data-dismiss=\"modal\">Cancelar</button>
                                            <button  type=\"button\"  class=\"btn btn-primary\" onclick=\"editarEtapa({$r['accid']})\"><span class=\"glyphicon glyphicon-plus\"></span> Salvar</button>
                                        </div>
                                        </form>
                                    </div>
                                </div>
                            </div>
                    </td>
                        </tr>
                    ";
                    $i++;
                }

            }else{
                $html .= "
                    <tr>
                        <td  colspan='6' style=\"vertical-align: middle;\">
                            <div class=\"alert alert-info text-center\">
                           Nenhum registro encontrado!
                        </div>
                        </td>
                    </tr>
                    
                ";
            }

            $html .= "
                    </tbody>
                        
                    </table>
                </div>
            </form>
            ";
            return $html;
        }

        public function excluirItemAgendaTrabalho($accid){
            $sql = "SELECT  * FROM sase.agendacomissaocoordenadora 
                      WHERE accid = {$accid} ";
            $dados  = $this->pegalinha($sql);
            $sql = "UPDATE sase.agendacomissaocoordenadora
                    SET accstatus = 'I', accdataalteracao= 'NOW()'
                    WHERE accid = {$accid}";

            $this->executar($sql);
            $this->commit();

            $dados = array_map("utf8_decode", $dados);

            $eacid          = $dados['eacid'];
            $accid           = $dados['accid'];
            $muncod         = $dados['muncod'];
            $accacao        = $dados['accacao'];
            $accresponsavel = $dados['accresponsavel'];
            $accprazo       =  $dados['accprazo'];
            $accobservacao  = $dados['accobservacao'];
            $usucpf         = $_SESSION['usucpf'];
            $accconcluida   = $dados['accconcluida'];




            $_sql_1 = "INSERT INTO sase.historicoagendacomissaocoordenadora(
                        eacid, muncod, haccacao, haccresponsavel, haccprazo, haccobservacao, usucpf, haccdatainclusao, accid ,haccmodificacao
                    )VALUES(
                        {$eacid}, '{$muncod}', '{$accacao}', '{$accresponsavel}', '{$accprazo}', '{$accobservacao}', '{$usucpf}', 'NOW()',{$accid}, 'Exclusão'
                );

            ";

            $this->executar($_sql_1);
            $this->commit();
        }
        
        public function salvarEspatasAgendaTrabalho( $dados ) {
            global $db;


            $eacid          = $dados['etapa'];
            $muncod         = $dados['muncod'];
            $accacao        = $dados['accacao'];
            $accresponsavel = $dados['accresponsavel'];
            $accprazo       = formata_data_sql( $dados['accprazo']);
            $accobservacao  = $dados['accobservacao'];
            $usucpf         = $_SESSION['usucpf'];



            $_sql_1 = "INSERT INTO sase.agendacomissaocoordenadora(
                                eacid,
                                muncod,
                                accacao,
                                accresponsavel,
                                accprazo,
                                accobservacao,
                                usucpf,
                                accdatainclusao
                            )VALUES(
                                {$eacid}, '{$muncod}', '{$accacao}', '{$accresponsavel}', '{$accprazo}', '{$accobservacao}', '{$usucpf}', 'NOW()');                        
                    ";

            $this->executar($_sql_1);
            $this->commit();

            $sql = "SELECT max(accid) from sase.agendacomissaocoordenadora";
            $accidinserido = $this->pegaum($sql);
            $_sql_2 = "
                        INSERT INTO sase.historicoagendacomissaocoordenadora(
                                eacid,
                                muncod,
                                haccacao,
                                haccresponsavel,
                                haccprazo,
                                haccobservacao,
                                usucpf,
                                haccdatainclusao,
                                haccstatus,
                                accid,
                                haccmodificacao
                            )VALUES(
                                {$eacid}, '{$muncod}', '{$accacao}', '{$accresponsavel}', '{$accprazo}', '{$accobservacao}', '{$usucpf}', 'NOW()', 'A' , {$accidinserido} , 'Inclusão' );
                        ";

            $this->executar($_sql_2);


            $isTrue = $this->commit();

            $_SESSION['msg'] = "Inserido com sucesso!";


            return $isTrue;

            if($isTrue == 1){
                    $db->sucesso( "principal/agendamento_trab","&acao=A&aba=edit&muncod={$muncod}", 'Operação realizada com sucesso!' );
            }elseif($isTrue == 2 ){
                $db->sucesso( "principal/agendamento_trab","&acao=A&aba=edit&muncod={$muncod}", 'Data Inválida!' );
            }else{
                $db->sucesso( "principal/agendamento_trab","&acao=A&aba=edit&muncod={$muncod}", "Ocorreu um erro ao executar operação!" );
            }
            
//           return $isTrue;
        }

        public function editarAgendaTrabalho( $dados ) {


            //header("Content-Type: text/html; charset=ISO-8859-1",true) ;
            $dados = array_map("utf8_decode", $dados);

            $eacid          = $dados['eacid'];
            $accid           = $dados['accid'];
            $muncod         = $dados['muncod'];
            $accacao        = $dados['accacao'];
            $accresponsavel = $dados['accresponsavel'];
            $accprazo       = formata_data_sql( $dados['accprazo']);
            $accobservacao  = $dados['accobservacao'];
            $usucpf         = $_SESSION['usucpf'];
            $accconcluida   = formata_data_sql( $dados['accconcluida']);


            if($accconcluida != ''){

                $insertaccconcluida =  " and  accconcluida  = '{$accconcluida}'";
            }else{
                $insertaccconcluida =  " and   accconcluida      = NULL";
            }


            $sql = "SELECT  accid FROM sase.agendacomissaocoordenadora 
                      WHERE accid = {$accid} AND 
                            muncod  = '{$muncod}' AND 
                            accacao  = '{$accacao}' AND 
                            accresponsavel  = '{$accresponsavel}' AND 
                            accprazo  = '{$accprazo}' AND
                            accstatus = 'A' {$insertaccconcluida}
 ";
            $resposnta  = $this->pegaum($sql);

            if($resposnta > 0){
                $_SESSION['msg'] = "Altere ao menos um campo!";
                return ;
            }

            if($accconcluida != ''){

                $upaccconcluida =  ", accconcluida      = '{$accconcluida}'";
            }else{
                $upaccconcluida =  ", accconcluida      = NULL";
            }

            $_sql_1 = "

            UPDATE  sase.agendacomissaocoordenadora
                        SET
                          eacid             =  {$eacid},
                          muncod             = '{$muncod}',
                          accacao           = '{$accacao}',
                          accresponsavel    = '{$accresponsavel}',
                          accprazo          = '{$accprazo}',
                          accobservacao     = '{$accobservacao}',
                          accstatus         = 'A',
                          usucpf            = '{$usucpf}',
                          accdataalteracao  = 'NOW()'
                         {$upaccconcluida}
                        WHERE accid = {$accid};


                INSERT INTO sase.historicoagendacomissaocoordenadora(
                        eacid, muncod, haccacao, haccresponsavel, haccprazo, haccobservacao, usucpf, haccdatainclusao, accid ,haccmodificacao
                    )VALUES(
                        {$eacid}, '{$muncod}', '{$accacao}', '{$accresponsavel}', '{$accprazo}', '{$accobservacao}', '{$usucpf}', 'NOW()',{$accid}, 'Alteração'
                );

            ";

            $this->executar($_sql_1);
            $this->commit();


            $_SESSION['msg'] = "Alterado com sucesso!";

            return true;
        }

        public function dadosHistorico($muncod, $eacid){
            global $db;
            $sql = <<<DML
               SELECT
                    mun.estuf,
                    mun.mundescricao,
                    acc.haccacao,
                    acc.haccresponsavel,
                    to_char(acc.haccprazo, 'DD/MM/YYYY') as accprazo,
                    acc.haccobservacao,
                    to_char(acc.haccconcluida, 'DD/MM/YYYY') as haccconcluida,
                    usu.usucpf,
                    usu.usunome,
                    to_char(ag.accdatainclusao, 'DD/MM/YYYY HH:MI:SS') as accdatainclusao,
                    case when acc.haccdatainclusao = acc.haccdataalteracao
                    then null 
                    else to_char(acc.haccdatainclusao, 'DD/MM/YYYY HH:MI:SS') end accdataalteracao,
                acc.haccmodificacao
                FROM sase.historicoagendacomissaocoordenadora acc
                inner join sase.agendacomissaocoordenadora ag  on ag.accid = acc.accid
                inner join territorios.municipio mun on acc.muncod = mun.muncod
                inner join seguranca.usuario usu on acc.usucpf = usu.usucpf
                WHERE  acc.muncod = '{$muncod}' AND acc.eacid = {$eacid}
                ORDER BY acc.haccdataalteracao desc ,ag.accid DESC
                
DML;


            $cabecalho = array("Estado", "Município", "Ação", "Responsável", "Prazo", "Observação", "Concluído em", "CPF", "Usuário", "Data de Inclusão", "Data de Alteração/Exclusão", "Operação");
            $alinhamento = array("left","left","left","left","left","left","left","left","left","left");
            $largura = array();
            $historico = $db->monta_lista($sql,$cabecalho,30,15,'N','','N','historico',$largura,$alinhamento);
            return $historico;
        }

        public function montaLegenda( Array $estuf ){
            global $db;
            $where = ( ( $estuf!='' && count($estuf)>0 ) ? " and m.estuf in ( '". ( implode( "','", $estuf ) ) ."' ) " : "" );

            $sql = <<<DML
                select
                    (
                        SELECT
                            count(DISTINCT m.*)
                        FROM territorios.municipio AS m
                        LEFT JOIN sase.agendacomissaocoordenadora AS a ON a.muncod = m.muncod
                        where 1=1
                        {$where}
                        AND accdatainclusao IS NOT NULL 
                        AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 1 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' AND accresponsavel != '' AND accprazo IS NOT NULL) ) )
                        --AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 1 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' OR accresponsavel != '' OR accprazo IS NOT NULL OR accobservacao != '') ) )
                        --AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 2 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' OR accresponsavel != '' OR accprazo IS NOT NULL OR accobservacao != '') ) )
                        --AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 3 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' OR accresponsavel != '' OR accprazo IS NOT NULL OR accobservacao != '') ) )
                        --AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 4 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' OR accresponsavel != '' OR accprazo IS NOT NULL OR accobservacao != '') ) )
                    ) as preenchido,

                    (
                        SELECT
                            count(DISTINCT m.*)
                        FROM territorios.municipio AS m
                        LEFT JOIN sase.agendacomissaocoordenadora AS a ON a.muncod = m.muncod
                        where 1=1
                        {$where}
                        AND accdatainclusao IS NULL
                    ) as naopreenchido
DML;
            $res = $db->pegaLinha($sql);

            $html = <<<HTML
                <div id="legendaMapaContainer">
                <div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
                <ul>
                    <li>
                        <table>
                            <tr>
                                <td>
                                    <span style='background:#00cc00;' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;
                                    <b>{$res['preenchido']}</b>&nbsp;&nbsp;
                                </td>
                                <td>Preenchido</td>
                            </tr>
                            <tr>
                                <td>
                                    <span style='background:#cc0000;' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;
                                    <b>{$res['naopreenchido']}</b>&nbsp;&nbsp;
                                </td>
                                <td>Não Preenchido</td>
                            </tr>
                        </table>
                    </li>
                </ul>
HTML;
            echo $html;
        }

        /**
         * Função para montar arquivo excel - Municípios
         * @access public
         * @param array $_POST
         * @return array
         */
        public function montaListaXls( $dados ){
            global $db;

             $perfis  = pegaPerfilGeral( $_SESSION['usucpf'] );
            $usucpf  = $_SESSION['usucpf'];
            $saseTec = PFLCOD_SASE_TECNICO;

            if( $dados['estuf'] != '' ){
                $and[] = "e.estuf = '{$dados['estuf']}'";
            }

            if( $dados['muncod'] != '' ){
                $and[] = "m.muncod = '{$dados['muncod']}'";
            }

            if( $dados['etapas'] != '' ){
                $and[] = "accdatainclusao IS NOT NULL 
                            AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 1 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' AND accresponsavel != '' AND accprazo IS NOT NULL) ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 1 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' OR accresponsavel != '' OR accprazo IS NOT NULL OR accobservacao != '') ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 2 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' OR accresponsavel != '' OR accprazo IS NOT NULL OR accobservacao != '') ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 3 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' OR accresponsavel != '' OR accprazo IS NOT NULL OR accobservacao != '') ) )
                            --AND (0 < (select distinct count(*) from sase.agendacomissaocoordenadora AS acc where eacid  = 4 and accstatus = 'A' AND acc.muncod = m.muncod AND ( accacao != '' OR accresponsavel != '' OR accprazo IS NOT NULL OR accobservacao != '') ) )                         ";
            }

            if( $and[0] != '' ){
                $_and = ' AND '.implode(' AND ', $and);
            }

            if( in_array( PFLCOD_SASE_SUPER_USUARIO, $perfis) || in_array( PFLCOD_SASE_ADMINISTRADOR, $perfis) ){
                $_where = "WHERE u.rpustatus = 'A' AND pflcod = {$saseTec} {$_and}";
            }elseif( in_array( PFLCOD_SASE_TECNICO, $perfis) ){
                $_where = "WHERE u.rpustatus = 'A' AND u.usucpf = '{$usucpf}' AND pflcod = {$saseTec} {$_and}";
            }

            $SQL = "
                SELECT  DISTINCT 
                        e.estuf||' - '||estdescricao as estado,
                        m.mundescricao,
                         CASE WHEN accid IS NOT NULL
                        THEN '<center>  S  </center>'
                        ELSE 'N'
                        END AS preenchido
                FROM territorios.municipio AS m
                JOIN territorios.estado AS e ON e.estuf = m.estuf
                JOIN sase.usuarioresponsabilidade AS u ON u.muncod = m.muncod
                LEFT JOIN sase.agendacomissaocoordenadora AS a ON a.muncod = u.muncod AND eacid  = 1 and accstatus = 'A' AND accacao != '' AND accresponsavel != '' AND accprazo IS NOT NULL AND accdatainclusao IS NOT NULL
                {$_where}
                ORDER BY 1, 3
            ";

            $cabecalho = array('Estado', 'Município','Todas as Etapas Preenchidas');
            $alinhamento = array('left','left','left', "left",'left','left');
            $larguras = array('5%','5%','75%', '10%', '5%', '5%');

            ob_clean();
            header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
            header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
            header("Pragma: no-cache");
            header("Content-type: application/xls; name=ssimec_sase_agenda_trabalho_" . date("Ymdhis") . ".xls");
            header("Content-Disposition: attachment; filename=simec_sase_agenda_trabalho_" . date("Ymdhis") . ".xls");
            header("Content-Description: MID Gera excel");

            $db->monta_lista($SQL,$cabecalho,100000,5,'N','','N','listaAgendaTrabalho',$larguras,$alinhamento);
        }
        
        /**
        * Monta legenda de acordo com os estados selecionados apresentando a quantidade de municipios envolvidos com a situação
        *
        * @author Sávio Resende
        * @param array $estuf
        * @return html
        */
       public function montaLegendaMunicipioExterno($estuf)
       {
        global $db; ?>
        <div id="legendaConteudo" style="display:block;float:right;width:285px;" class="topo_bootstrap_off">
            <div id="tituloLegenda">
                <div style="float:left;"><h5 style="font-size:14px !important;margin:2px !important;margin-left:0px !important;">&nbsp;Legenda:</h5></div>
                <div style="float:right;"></div>
                <div style="clear:both;height:1px;">&nbsp;</div>
            </div>
            <ul>
                <?php
                
                require_once dirname(str_replace('/Model', '',(str_replace('\Model', '', __FILE__)))) . '/Mapa/Metadados/DadoAgendaComissaoCoordenadoraExterno.php';
                $classeDados = 'DadoAgendaComissaoCoordenadoraExterno';
                $legenda = (new $classeDados())->dadosDaLegenda();
                foreach ($legenda as $value) {
                    echo <<<HTML
                        <li>
                            <table>
                                <tr>
                                    <td style="width:47px;">
                                        <span style="background:{$value['cor']};" class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>{$value['total']}</b>&nbsp;&nbsp;
                                    </td>
                                    <td>{$value['descricao']}</td>
                                </tr>
                            </table>
                        </li>
HTML;
                }
                ?>
            </ul>
        </div>
    <?php
    }

        /**
         * Tratamento de regras de perfis para geração de filtros para 'montaListaQuery'
         * @param string $usucpf - CPF de login
         * @param string $sql - sql da lista
         * @param constante $tipo  - perfil DICOPE ou DIVAPE, se não declarar exibirá todos os estados ou municípios vinculados ao CPF.
         */
        public function trataRegrasPerfis( $usucpf, $sql ,$tipo = null ){

            $perfis = pegaPerfilGeral( $usucpf );
            $tipoPerfil = ($tipo != null) ? " AND p.pflcod in (".implode(",", $tipo)." )" : '';
            $sql_temp = " SELECT
                                    p.pflcod AS pflcod,
                                    ur.estuf AS estuf,
                                    ur.muncod AS muncod
                             FROM seguranca.usuario u
                             INNER JOIN seguranca.perfilusuario pu on pu.usucpf = u.usucpf
                             INNER JOIN seguranca.perfil p on p.pflcod = pu.pflcod and p.sisid = 183
                             INNER JOIN sase.usuarioresponsabilidade ur on ur.pflcod = p.pflcod and ur.usucpf = u.usucpf and ur.rpustatus = 'A'
                            where u.usucpf = '".$usucpf."'
                            {$tipoPerfil}; ";
            $responsabilidades = $this->carregar( $sql_temp );
            // resgata municipios
            $municipios = array();
            if( !empty($responsabilidades) )
            foreach ($responsabilidades as $key => $value) {

                    if( !empty($value['muncod']) )
                            array_push($municipios, $value['muncod']);
            }

            // resgata estados
            $estados = array();
            if( !empty($responsabilidades) )
            foreach ($responsabilidades as $key => $value) {

                    if( !empty($value['estuf']) )
                            array_push($estados, $value['estuf']);
            }

            $OR = false;

            if( array_search(PFLCOD_SASE_EXECUTIVO, $perfis) !== false
                    && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false ){
                    $sql .= " AND ( e.estuf in ('".(implode('\',\'',$estados))."') )";
                    $OR = true;
            }

            if( array_search(PFLCOD_SASE_SUPERVISOR_GERAL, $perfis) !== false
                    && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false ){
                    $sql .= " AND ( e.estuf in ('".(implode('\',\'',$estados))."') )";
                    $OR = true;
            }

            if( ( array_search(PFLCOD_SASE_SUPERVISOR, $perfis)!==false
                    || array_search(PFLCOD_SASE_TECNICO, $perfis)!==false)
                    && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false ){

                    if( $OR ){
                            $sql = substr($sql, 0, -1);
                            $sql .= " OR m.muncod in ('".(implode("','",$municipios))."') )";
                    }else{
                            $sql .= " AND ( m.muncod in ('".(implode("','",$municipios))."') )";
                    }
            }

            return $sql;
    }
    
    public function buscaMunicipio($muncod)
    {
        $sql =  "select estuf || ' - ' || mundescricao as municipio
                 from territorios.municipio 
                 where muncod = '{$muncod}'";
                 
        return $this->pegaUm($sql);
    }
}
