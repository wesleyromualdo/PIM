<?php

namespace Modules\Seguranca\Entities;

use Modules\Seguranca\Entities\Base\ExportarSistemaBase;
use Modules\Seguranca\Entities\Sistema;
use Modules\Seguranca\Entities\Perfil;
use Modules\Seguranca\Entities\PerfilUsuario;
use Modules\Seguranca\Entities\Menu;
use Modules\Workflow\Entities\EstadoDocumento;

class ExportarSistema extends ExportarSistemaBase
{
    public static function montaSqlDadosTabela($shema, $table)
    {
        $tableSistema = \DB::select("SELECT
                                        column_name,
                                        is_nullable,
                                        data_type,
                                        character_maximum_length
                                    FROM
                                        information_schema.columns
                                    WHERE
                                        table_schema = '{$shema}'
                                    AND
                                        table_name = '{$table}'");


        return $tableSistema;
    }

    public static function montaInsertSegurancaSistema($sisid){

        $dataSistema = Sistema::find($sisid);

        $tableSistema = ExportarSistema::montaSqlDadosTabela('seguranca', 'sistema');

        if($dataSistema){

            foreach($tableSistema as $table){

                if(isset($dataSistema[$table->column_name])){

                    $arCampos[]  = $table->column_name;
                    $arValores[] = empty($dataSistema[$table->column_name]) ? 'null' : "'".$dataSistema[$table->column_name]."'";

                }

            }

            if(is_array($arCampos) && is_array($arValores)){

                $insertSistema = "INSERT INTO seguranca.sistema<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";

            }
        }

        return $insertSistema;
    }

    public static function montaInsertSegurancaPerfil($sisid){

        $dataPerfil = Perfil::where('sisid','=',$sisid)->get();

        $tablePerfil = ExportarSistema::montaSqlDadosTabela('seguranca', 'perfil');

        if($dataPerfil){

             $insertPerfil = '';

            foreach($dataPerfil as $data){

                unset($table);
                unset($arCampos);
                unset($arValores);

                foreach($tablePerfil as $table){

                    foreach($data->attributes as $column_name => $column_value){

                        if($table->column_name == $column_name && $table->column_name != 'pflcod'){

                            $arCampos[]  = $table->column_name;
                            $arValores[] = empty($data[$table->column_name]) ? 'null' : "'".$data[$table->column_name]."'";

                        }

                    }

                }

                if(is_array($arCampos) && is_array($arValores)){

                    $insertPerfil .= "INSERT INTO seguranca.perfil<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";

                }

            }

        }
        return $insertPerfil;
    }

    public static function montaInsertSegurancaMenu($sisid){

        $dataMenu = Menu::where('sisid','=',$sisid)->orderBy('mnucod')->get();

        $tableMenu = ExportarSistema::montaSqlDadosTabela('seguranca', 'menu');

        $retornoMenu='';

        if ($dataMenu) {

            $insertMenu = $updateMenuPai = '';

            foreach($dataMenu as $data){

                unset($table, $arCampos, $arValores);

                foreach($tableMenu as $table){

                    foreach($data->attributes as $column_name => $column_value){

                        if($table->column_name == $column_name && $table->column_name != 'mnuid'){

                            $arCampos[]  = $table->column_name;

                            if($table->column_name == 'mnuidpai' && $data[$table->column_name]>0){

                                $arValores[] = 'null';

                                $rsMenuPai = Menu::find($data[$table->column_name]);

                                $stMenuPai = "(SELECT mnuid FROM seguranca.menu WHERE sisid = {$_REQUEST['sisid']} and mnudsc = '{$rsMenuPai['mnudsc']}' ".(empty($rsMenuPai->mnulink) ? "and (mnulink is null or trim(mnulink) = '')" : "and mnulink = '{$rsMenuPai->mnulink}'" )." and mnucod = '{$rsMenuPai->mnucod}')";

                                $updateMenuPai .= "UPDATE seguranca.menu SET mnuidpai = {$stMenuPai} WHERE mnudsc = '{$data['mnudsc']}' ".(empty($data['mnulink']) ? "and (mnulink is null or trim(mnulink) = '')" : "and mnulink = '{$data['mnulink']}'" )." and mnucod = '{$data['mnucod']}' and sisid = {$_REQUEST['sisid']};<br/><br/>";

                            }else{

                                $arValores[] = empty($data[$table->column_name]) ? 'null' : "'".$data[$table->column_name]."'";
//                                dd($arValores);
                            }
                        }

                    }

                }
                if(is_array($arCampos) && is_array($arValores)){

                    $insertMenu .= "INSERT INTO seguranca.menu<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";

                }
            }
        }

        $retornoMenu .=$insertMenu;
        $retornoMenu .=$updateMenuPai;

        return $retornoMenu;
    }

    public static function montaInsertPerfilMenu($sisid){

        $dataPerfilMenu = \DB::select("select 
                                        pm.*, 
                                        pf.pfldsc, 
                                        mn.mnucod, 
                                        mn.mnudsc, 
                                        mn.mnulink 
                                    from seguranca.perfilmenu pm
                                    join seguranca.perfil pf on pf.pflcod = pm.pflcod
 			                        join seguranca.menu mn on mn.mnuid = pm.mnuid
 			                        where pf.sisid = {$sisid} and mn.sisid = {$sisid}");

        $tableMenu = ExportarSistema::montaSqlDadosTabela('seguranca', 'perfilmenu');

        if($dataPerfilMenu){

            $insertPerfilMenu = '';

            foreach($dataPerfilMenu as $data){

                unset($table);
                unset($arCampos);
                unset($arValores);

                foreach($tableMenu as $table){

                    foreach($data as $column_name => $column_value){

                        if($table->column_name == $column_name){

                            $arCampos[]  = $table->column_name;

                            if($table->column_name == 'mnuid' && $data->{$table->column_name}>0){

                                $arValores[] = "(select mnuid from seguranca.menu where mnucod = '{$data->mnucod}' and mnudsc = '{$data->mnudsc}' ".(empty($data->mnulink) ? "and mnulink is null" : "and mnulink = '{$data->mnulink}'" )." and sisid = {$sisid})";

                            }else if($table->column_name == 'pflcod' && $data->{$table->column_name}>0){

                                $arValores[] = "(select pflcod from seguranca.perfil where pfldsc = '{$data->pfldsc}' and sisid = {$sisid})";

                            }else{

                                $arValores[] = empty($data->{$table->column_name}) ? 'null' : "'".$data->{$table->column_name}."'";

                            }

                        }

                    }

                }

                if(is_array($arCampos) && is_array($arValores)){

                    $insertPerfilMenu .= "INSERT INTO seguranca.perfilmenu<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";

                }

            }

        }

        return $insertPerfilMenu;

    }

    public static function montaInsertPerfilUsuario($sisid){

        $dataPerfilUsuario = PerfilUsuario::select(['*'])
                                ->join(Perfil::getTableName() . ' as p ', PerfilUsuario::getTableName() . '.pflcod', '=', 'p.pflcod')
                                ->where('p.sisid', '=', $sisid)
                                ->where('p.pfldsc', '=', 'Super Usuário')->get();

        $tablePerfilUsuario = ExportarSistema::montaSqlDadosTabela('seguranca', 'perfilusuario');

        if($dataPerfilUsuario){

            $insertPerfilUsuario = '';

            foreach($dataPerfilUsuario as $data){

                unset($table);
                unset($arCampos);
                unset($arValores);

                foreach($tablePerfilUsuario as $table){

                    foreach($data->attributes as $column_name => $column_value){

                        if($table->column_name == $column_name){

                            $arCampos[]  = $table->column_name;

                            if($table->column_name == 'pflcod' && $data['attributes'][$table->column_name]>0){

                                $arValores[] = "(select pflcod from seguranca.perfil where pfldsc = '{$data['attributes']['pfldsc']}' and sisid = {$sisid})";

                            }else{

                                $arValores[] = empty($data['attributes'][$table->column_name]) ? 'null' : "'".$data['attributes'][$table->column_name]."'";

                            }

                        }

                    }

                }

                if(is_array($arCampos) && is_array($arValores)){

                    $insertPerfilUsuario .= "INSERT INTO seguranca.perfilusuario<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";

                }

            }

        }

        return $insertPerfilUsuario;

    }
    public static function montaInsertTipoDocumento($sisid){

        $dataWfTipodocumento = \Modules\Workflow\Entities\TipoDocumento::where('sisid', '=', $sisid)->get()->toArray();

        $tableWfTipodocumento = ExportarSistema::montaSqlDadosTabela('workflow', 'tipodocumento');

        if($dataWfTipodocumento){

            $insertWfTipodocumento = '';

            foreach($dataWfTipodocumento as $data){

                unset($table);
                unset($arCampos);
                unset($arValores);

                foreach($tableWfTipodocumento as $table){

                    foreach($data as $column_name => $column_value){

                        if($table->column_name == $column_name /*&& $table['column_name'] != 'tpdid'*/){

                            $arCampos[]  = $table->column_name;
                            $arValores[] = empty($data[$table->column_name]) ? 'null' : "'".$data[$table->column_name]."'";

                        }

                    }

                }

                if(is_array($arCampos) && is_array($arValores)){

                    $insertWfTipodocumento .= "INSERT INTO workflow.tipodocumento<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";

                }

            }

        }

        return $insertWfTipodocumento;

    }

    public static function montaInsertEstadoDocumento($sisid){

        $dataWfEstadoDocumento = \DB::select("SELECT est.*, tpd.tpdid, tpddsc FROM workflow.estadodocumento est
                                                JOIN workflow.tipodocumento tpd ON tpd.tpdid = est.tpdid
                                                WHERE tpd.sisid = {$sisid} and est.esdstatus = 'A'");

        $tableWfEstadoDocumento = ExportarSistema::montaSqlDadosTabela('workflow', 'estadodocumento');

        if($dataWfEstadoDocumento){

            $insertWfEstadoDocumento = '';

            foreach($dataWfEstadoDocumento as $data){

                unset($table);
                unset($arCampos);
                unset($arValores);

                foreach($tableWfEstadoDocumento as $table){

                    foreach($data as $column_name => $column_value){

                        if($table->column_name == $column_name && $table->column_name != 'esdid'){

                            $arCampos[]  = $table->column_name;

                            if($table->column_name == 'tpdid'){

//                            $arValores[] = "(SELECT tpdid FROM workflow.tipodocumento WHERE sisid = {$_REQUEST['sisid']} AND tpddsc = '{$data['tpddsc']}')";
                                $arValores[] = $data->tpdid;

                            }else{

                                $arValores[] = empty($data->{$table->column_name}) ? 'null' : "'".$data->{$table->column_name}."'";

                            }

                        }

                    }

                }

                if(is_array($arCampos) && is_array($arValores)){

                    $insertWfEstadoDocumento .= "INSERT INTO workflow.estadodocumento<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";

                }

            }

        }

        return $insertWfEstadoDocumento;
    }

    public static function montaInsertAcaoEstadoDocumento($sisid){

        $dataWfAcaoEstadoDocumento = \DB::select("SELECT aed.*, eso.esddsc as esddscorigem, esd.esddsc as esddscdestino, tpd.tpdid, tpd.tpddsc FROM workflow.acaoestadodoc aed
                                                JOIN workflow.estadodocumento eso ON eso.esdid = aed.esdidorigem and eso.esdstatus = 'A'
                                                JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdiddestino and esd.esdstatus = 'A'
                                                JOIN workflow.tipodocumento tpd ON tpd.tpdid = eso.tpdid AND tpd.tpdid = esd.tpdid
                                                WHERE tpd.sisid = {$sisid} ");

        $tableWfAcaoEstadoDocumento = ExportarSistema::montaSqlDadosTabela('workflow', 'acaoestadodoc');

        if($dataWfAcaoEstadoDocumento){

            $insertWfAcaoEstadoDocumento = '';

            foreach($dataWfAcaoEstadoDocumento as $data){

                unset($table);
                unset($arCampos);
                unset($arValores);

                foreach($tableWfAcaoEstadoDocumento as $table){

                    foreach($data as $column_name => $column_value){

                        if($table->column_name == $column_name && $table->column_name != 'aedid'){

                            $arCampos[]  = $table->column_name;

                            if($table->column_name == 'esdidorigem'){

                                $arValores[] = "(SELECT esdid FROM workflow.estadodocumento esd
                                                    JOIN workflow.tipodocumento tpd ON tpd.tpdid = esd.tpdid
                                                    WHERE tpd.sisid = {$sisid} AND esd.esddsc = '{$data->esddscorigem}' AND tpd.tpdid = '{$data->tpdid}')";

                            }else if($table->column_name == 'esdiddestino'){

                                $arValores[] = "(SELECT esdid FROM workflow.estadodocumento esd
                                                    JOIN workflow.tipodocumento tpd ON tpd.tpdid = esd.tpdid
                                                    WHERE tpd.sisid = {$sisid} AND esd.esddsc = '{$data->esddscdestino}' AND tpd.tpdid = '{$data->tpdid}')";

                            }else{

                                $arValores[] = empty($data->{$table->column_name}) ? 'null' : "'".$data->{$table->column_name}."'";

                            }

                        }

                    }

                }

                if(is_array($arCampos) && is_array($arValores)){

                    $insertWfAcaoEstadoDocumento .= "INSERT INTO workflow.acaoestadodoc<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";

                }

            }

        }
        return $insertWfAcaoEstadoDocumento;
    }

    public static function montaInsertEstadoDocumentoPerfil($sisid){

        $dataWfEstadoDocumentoPerfil = \DB::select("SELECT edp.*, pfl.pfldsc, eso.esddsc as esddscorigem, esd.esddsc as esddscdestino, tpd.tpdid, tpd.tpddsc,
                                                    aeddscrealizar, aeddscrealizada FROM workflow.estadodocumentoperfil edp
                                                    JOIN workflow.acaoestadodoc aed ON aed.aedid = edp.aedid
                                                    JOIN workflow.estadodocumento eso ON eso.esdid = aed.esdidorigem and eso.esdstatus = 'A'
                                                    JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdiddestino  and esd.esdstatus = 'A'
                                                    JOIN seguranca.perfil pfl ON pfl.pflcod = edp.pflcod
                                                    JOIN workflow.tipodocumento tpd ON tpd.tpdid = eso.tpdid AND tpd.tpdid = esd.tpdid
                                                    WHERE tpd.sisid = {$sisid} ");

        $tableWfEstadoDocumentoPerfil = ExportarSistema::montaSqlDadosTabela('workflow', 'estadodocumentoperfil');

        if($dataWfEstadoDocumentoPerfil){

            $insertWfEstadoDocumentoPerfil = '';

            foreach($dataWfEstadoDocumentoPerfil as $data){

                unset($table);
                unset($arCampos);
                unset($arValores);

                foreach($tableWfEstadoDocumentoPerfil as $table){

                    foreach($data as $column_name => $column_value){

                        if($table->column_name == $column_name){

                            $arCampos[]  = $table->column_name;

                            if($table->column_name == 'aedid'){

                                $arValores[] = "(
											SELECT aedid FROM workflow.acaoestadodoc
											WHERE esdidorigem = (SELECT esdid FROM workflow.estadodocumento esd
																JOIN workflow.tipodocumento tpd ON tpd.tpdid = esd.tpdid
																WHERE tpd.sisid = {$sisid} and tpd.tpdid = '{$data->tpdid}' AND esd.esddsc = '{$data->esddscorigem}')
											AND esdiddestino = (SELECT esdid FROM workflow.estadodocumento esd
																JOIN workflow.tipodocumento tpd ON tpd.tpdid = esd.tpdid
																WHERE tpd.sisid = {$sisid} and tpd.tpdid = '{$data->tpdid}' AND esd.esddsc = '{$data->esddscdestino}')
											AND aeddscrealizar = '{$data->aeddscrealizar}'
											AND aeddscrealizada = '{$data->aeddscrealizada}'
											)";

                            }else if($table->column_name == 'pflcod'){

                                $arValores[] = "(SELECT pflcod FROM seguranca.perfil WHERE sisid = {$sisid} AND pfldsc = '{$data->pfldsc}')";

                            }
                        }
                    }
                }

                if(is_array($arCampos) && is_array($arValores)){

                    $insertWfEstadoDocumentoPerfil .= "INSERT INTO workflow.estadodocumentoperfil<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";
                }
            }
        }
        return $insertWfEstadoDocumentoPerfil;
    }
    public static function montaInsertUsuarioResponsabilidade($sisid){

        $dataSistema = Sistema::find($sisid);

        $dataUsuarioResponsabilidade = \DB::select("SELECT ur.*, pfldsc FROM {$dataSistema['sisdiretorio']}.usuarioresponsabilidade ur
                                                JOIN seguranca.perfil pf ON pf.pflcod = ur.pflcod
                                                WHERE pf.sisid = {$sisid} ");

        $tableUsuarioResponsabilidade = ExportarSistema::montaSqlDadosTabela($dataSistema['sisdiretorio'], 'usuarioresponsabilidade');

        if($dataUsuarioResponsabilidade){

            $insertUsuarioResponsabilidade = '';

            foreach($dataUsuarioResponsabilidade as $data){

                unset($table);
                unset($arCampos);
                unset($arValores);

                foreach($tableUsuarioResponsabilidade as $table){

                    foreach($data as $column_name => $column_value){

                        if($table->column_name == $column_name){

                            $arCampos[]  = $table->column_name;

                            if($table->column_name == 'pflcod'){

                                $arValores[] = "(SELECT pflcod FROM seguranca.perfil WHERE sisid = {$sisid} AND pfldsc = '{$data->pfldsc}')";

                            }else if($table->column_name == 'rpudata_inc'){

                                $arValores[] = 'now()';

                            }else{

                                $arValores[] = empty($data[$table->column_name]) ? 'null' : "'".$data[$table->column_name]."'";

                            }
                        }
                    }
                }
                if(is_array($arCampos) && is_array($arValores)){
                    $insertUsuarioResponsabilidade .= "INSERT INTO {$dataSistema['sisdiretorio']}.usuarioresponsabilidade<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";
                }
            }
        }

        return $insertUsuarioResponsabilidade;
    }
}