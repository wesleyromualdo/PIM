<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 9/26/2017
 * Time: 2:55 PM
 */

class Par3_Model_PerfilUsuario extends Modelo{

    public function getSqlPerfilTecnicoObrasMontaCombo($statusTecnico = null){
        $modelUsuarioResponsabilidade = new Par3_Model_UsuarioResponsabilidade();
        $perfil = $modelUsuarioResponsabilidade::ANALISTA_TECNICO_CGEST; //PAR_PERFIL_ENGENHEIRO_FNDE no par antigo
        $sql = '';
        if(is_null($statusTecnico ) || $statusTecnico == 'T'){ //carrega todos os técnicos
            $sql = "SELECT  u.usucpf as codigo, u.usunome as descricao
         FROM seguranca.perfilusuario pu
         INNER JOIN seguranca.usuario u ON pu.usucpf = u.usucpf
         WHERE pu.pflcod IN ({$perfil})
                                        ORDER BY u.usunome";

        }
            elseif ($statusTecnico == 'P'){ //Selecina os técnicos inativos
            $sql = "SELECT u.usucpf as codigo, u.usunome as descricao
                                        FROM seguranca.perfilusuario pu
                                        INNER JOIN seguranca.usuario u ON pu.usucpf = u.usucpf
                                        WHERE pu.pflcod IN ({$perfil}) AND u.usustatus = 'P'
                                        ORDER BY u.usunome";
        }
        return $sql;
    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $cpf String
     * @return array
     * @throws Exception
     * @description <Função utilizada para retornar string contendo todos os perfis utilizando string cpf como parâmetro>
     */
    public function getPerfisUsuarioByCPF($cpf){
        $sql = "select * from seguranca.perfilusuario as perfis where perfis.usucpf = '{$cpf}' ;";
        try{
            $result = $this->carregar($sql);

            return $result;
        }catch (Exception $e){
            throw new Exception($e->getMessage());
        }
    }

}